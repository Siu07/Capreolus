//https://github.com/Siu07/Capreolus/tree/master/Capreolus
/*
todo:
 Done 100%: average last 5 seconds of reading to give consistant result
 Done 50%: make user input possible                                            Note: Buy a better rotary encoder, consider a keypad matrix
 auto turn off backlight                                             Note: add timeout, think about pwm to fade screen out.
 upgrade power supply                                                Note: 1A should be able to keep 12v steady. Need to get 2.1mm socket
 Done 75%: make routines more efficient                              Note: Using TimeAlarms to provide screen updates at regular intervals rather than slow down processing code.
 Done 90%:add real menu                                                       Note: Experiment with M2tklib more. Probably require complete restructure of code.
 integrate data logging
 integrate phone support
 Done 100%: only map input reading for display, use unmapped for calculations
 Done 90%: resume on power loss.                                     Note: Rudimentory systems, No run time continuation, starts from begining of last recorded step.
 Done 100%: test "doubleMap" function.
 Done 100%: Note: Add "first boot" flag (with version number) to repopulate all EEPROM data on update or first run. 
 */
 /*
 EEPROM Memory Allocation:
  heKp       = 0      Heater
  heKi       = 4
  heKd       = 8
  coKp       = 12    Cooler
  coKi       = 16
  coKd       = 20
  huKp       = 24    Humidifier
  huKi       = 28
  huKd       = 32
  deKp       = 36    Dehumidifier
  deKi       = 40
  deKd       = 44
  bootFlag   = 48    first run of new version flag
  programme  = 52    Resume program on power loss
  prgStep    = 56    Resume step on power loss
  runTimer   = 60    Program timer
 */
#include <MenuBackend.h>         //Menu element
#include <LiquidCrystal.h>       //LCD 20x4
#include <PID_v1.h>              //
#include <PID_AutoTune_v0.h>  // Hope to integrate, but not yet.
#include <EEPROMex.h>            //Saving double varibles to EEPROM
#include <Time.h>                //Counting from boot time
#include <TimeAlarms.h>          //Basic multithreading
#include <Bounce2.h>             //Cleaning User inputs
#include <math.h>                //mapping numbers between differing scales

char Version[] = "07/05/14";

boolean encoderChris = true;  //my encoder or davids. Make False before publish
double bootFlag = 1;  //version number, increment if PID values are changed
//rotary encoder & user inputs
enum PinAssignments {
  encoderPin1 = 2,   // scroll right
  encoderPin2 = 3,   // scroll left
  buttonPin = 20,    // Select
  backButton = 22    // back
};
int sum = 0;  //encoder sum
//int oldValue = 0;  //Detect if encoder has spun
volatile int lastEncoded = 0; //Encoder sum (david)
volatile unsigned int encoderValue = 0;  // a counter for the dial
unsigned int lastReportedPos = 1;   // change management
static boolean rotating=false;      // debounce management
// interrupt service routine vars
boolean A_set = false;              
boolean B_set = false;
//Display
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
//EEPROMex
const int memBase = 350;
//PID
int RH, Temp;
int programme, prgStep, prgHour, prgMin, prgSec;
double deadband = 0.5; //set the deadband to prevent PID controllers fighting.
int hours = 0, mins =0, secs = 0;
int buttonState = 0;
long t = 0;
//menu
int menuFlag;
boolean calculating = false;
char name[] = "error";
//Averageing inputs Temperature
const int numReadings = 10;
double readingsTemp[numReadings];      // the readings from the analog input
int index = 0;                  // the index of the current reading
double totalTemp = 0;                  // the running total
double averageTemp = 0;                // the average
//Averageing inputs RH
double readingsRH[numReadings];      // the readings from the analog input
double totalRH = 0;                  // the running total
double averageRH = 0;                // the average
//Arduino hardware IO
int tempPin = A0;    //Define input pins
int rhPin = A1;
int hePin = 40;     //Define output pins
int coPin = 10;
int huPin = 9;
int dePin = 8;
//User Input
int lastDebounceTime = 0;  // the last time the output pin was toggled
int debounceDelay = 50;    // the debounce time; increase if the output flickers
int storedPrg = 0;
//boolean menuSelect = true;  //debounced select, use or remove
//boolean menuReturn = true;

//Programme One - Snack Salami
int prgOne[8] = {
  2, 36, 6, 6, 6, 6, 36, 144}; // Hours
int prgOneRH[8] = {
  85, 90, 88, 85, 82, 80, 80, 80}; // Relative Humidity
int prgOneTemp[8] = {
  22, 23, 22, 20, 17, 14, 12, 10}; // Temperature
// Programme Two - Saucison Sec
int prgTwo[8] = {
  2, 48, 6, 6, 6, 12, 72, 144}; // Hours
int prgTwoRH[8] = {
  85, 90, 88, 85, 82, 80, 80, 80}; // Relative Humidity
int prgTwoTemp[8] = {
  22, 25, 22, 20, 17, 14, 12, 10}; // Temperature
// Programme three - Bacon
int prgThree[8] = {
  2, 12, 3, 3, 3, 3, 3, 9}; // Hours
int prgThreeRH[8] = {
  85, 90, 88, 85, 82, 80, 80, 80}; // Relative Humidity
int prgThreeTemp[8] = {
  22, 25, 22, 20, 17, 14, 12, 10}; // Temperature
// Programme Four - Large Meat
int prgFour[8] = {
  30, 45, 45, 30, 30, 30, 30, 30}; // Mins
int prgFourRH[8] = {
  80, 85, 90, 85, 80, 75, 70, 65}; // Relative Humidity
int prgFourTemp[8] = {
  18, 23, 25, 22, 19, 16, 13, 10}; // Temperature
//Holding Program
int prgFive[8] = {
  24, 24, 24, 24, 24, 24, 24, 24}; // Hours
int prgFiveRH[] = {
  80, 80, 80, 80, 80, 80, 80, 80}; // Relative Humidity
int prgFiveTemp[8] = {
  10, 10, 10, 10, 10, 10, 10, 10}; // Temperature

long runTimer = 0;    //milli's since program start
//heater                                      Define default PID parameters
int windowSize = 10000;
unsigned long windowStartTime;
double heInput, heOutput, heSetpoint;
double heKp = 1, heKi = 1, heKd = 10;
//cooler                                    
double coInput, coOutput, coSetpoint;
double coKp = 0.25, coKi = 0.1, coKd = 0; //coKp = 0.25, coKi = 0.25, coKd = 5;
//humidifier
double huInput, huOutput, huSetpoint;
double huKp = 0.25, huKi = 0.25, huKd = 2.5;
//dehumidifier
double deInput, deOutput, deSetpoint;
double deKp = 0.5, deKi = 0.25, deKd = 5;
//initilize PIDs
PID heater(&heInput, &heOutput, &heSetpoint, heKp, heKi, heKd, DIRECT);    
PID cooler(&coInput, &coOutput, &coSetpoint, coKp, coKi, coKd, REVERSE);
PID humidifier(&huInput, &huOutput, &huSetpoint, huKp, huKi, huKd, DIRECT);
PID dehumidifier(&deInput, &deOutput, &deSetpoint, deKp, deKi, deKd, REVERSE);
//Menu  Elements
MenuBackend menu = MenuBackend(menuUseEvent,menuChangeEvent);
  MenuItem snackSalami = MenuItem("Snack Salami");
    MenuItem runTime = MenuItem("Run");
  MenuItem saucionSec = MenuItem("Saucion Sec");
    //runTime
  MenuItem bacon = MenuItem("Bacon");
    //runTime
  MenuItem largeMeat = MenuItem("Large Meat");
    //runTime
  MenuItem Holding = MenuItem("Holding");
    //runTime
  MenuItem prgSettings = MenuItem("Program Settings");
    MenuItem SetssnackSalami = MenuItem("Snack Salami ");
      MenuItem hourSets = MenuItem("Hours");
        //Time settings
      MenuItem humidSets = MenuItem("RH");
        //humidity settings
      MenuItem tempSets = MenuItem("Temperature");
        //temperature settings
    MenuItem SetssaucionSec = MenuItem("Saucion Sec ");
        //Time settings
        //humidity settings
        //temperature settings
    MenuItem Setsbacon = MenuItem("Bacon ");
        //Time settings
        //humidity settings
        //temperature settings
    MenuItem SetslargeMeat = MenuItem("Large Meat ");
        //Time settings
        //humidity settings
        //temperature settings
    MenuItem SetsHolding = MenuItem("Holding ");
        //Time settings
        //humidity settings
        //temperature settings
  MenuItem PIDSettings = MenuItem("PID Settings");
    MenuItem prgHe = MenuItem("Heater");
      MenuItem Phe = MenuItem("Phe");
        //P settings
      MenuItem Ihe = MenuItem("Ihe");
        //I settings
      MenuItem Dhe = MenuItem("Dhe");
        //D settings
    MenuItem prgCo = MenuItem("Cooler");
      MenuItem Pco = MenuItem("Pco");
        //P settings
      MenuItem Ico = MenuItem("Ico");
        //I settings
      MenuItem Dco = MenuItem("Dco");
        //D settings
    MenuItem prgHu = MenuItem("Humidifier");
      MenuItem Phu = MenuItem("Phu");
        //P settings
      MenuItem Ihu = MenuItem("Ihu");
        //I settings
      MenuItem Dhu = MenuItem("Dhu");
        //D settings
    MenuItem prgDe = MenuItem("Dehumidifier");
      MenuItem Pde = MenuItem("Pde");
        //P settings
      MenuItem Ide = MenuItem("Ide");
        //I settings
      MenuItem Dde = MenuItem("Dde");
        //D settings
  MenuItem versionI = MenuItem("Version Info");

void setup() {
  Serial.begin(9600);
  lcd.begin(20, 4);
  //lcd.setCursor(0, 0);
  //lcd.print("Booting"); //Pointless as it boots so fast
  EEPROM.setMemPool(memBase, EEPROMSizeMega); //minimum and maximum address to use. To be used with address allocation.
  //Timing                                                                                       
   setTime(0,0,0,1,1,13); // set time to Saturday 0:00:00am Jan 1 2013   // setTime to be synced to RTC
  //modes
  pinMode(40, OUTPUT);    //Heater output is digital, not PWM
    //rotary encoder
  pinMode(encoderPin1, INPUT); 
  pinMode(encoderPin2, INPUT); 
  pinMode(buttonPin, INPUT);
  pinMode(backButton, INPUT);
 // turn on pullup resistors
  digitalWrite(encoderPin1, HIGH);
  digitalWrite(encoderPin2, HIGH);
  digitalWrite(buttonPin, HIGH);
  digitalWrite(backButton, HIGH);
// encoder pin on interrupt 0 (pin 2)
  attachInterrupt(0, doEncoderA, CHANGE);
// encoder pin on interrupt 1 (pin 3)
  attachInterrupt(1, doEncoderB, CHANGE);
  //PID
  windowStartTime = millis();
  heater.SetOutputLimits(0, windowSize);
  heater.SetMode(AUTOMATIC); //Should be AUTOMATIC, only manual during testing
  cooler.SetMode(AUTOMATIC);
  humidifier.SetMode(AUTOMATIC);
  dehumidifier.SetMode(AUTOMATIC);
  //output RH and Temp values to PID and initilise
  lcd.clear();
  Alarm.timerOnce(1, printscr);  //display every second when a program is running rather than make everything else pause
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readingsTemp[thisReading] = 0;  
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readingsRH[thisReading] = 0;
  heSetpoint = 10;
  huSetpoint = 80;
    //Serial.println("Save as a .CSV file");
  Serial.println("Date,HeaterKp,HeaterKi,HeaterKd,CoolerKp,CoolerKi,CoolerKd,HumididiferKp,HumididiferKi,HumididiferKd,DehumidifierKp,DehumidifierKi,DehumidifierKd");
  Serial.print("02/09/13,");
  Serial.print(heKp);
  Serial.print(",");
  Serial.print(heKi);
  Serial.print(",");
  Serial.print(heKd);
  Serial.print(",");
  Serial.print(coKp);
  Serial.print(",");
  Serial.print(coKi);
  Serial.print(",");
  Serial.print(coKd);
  Serial.print(",");
  Serial.print(huKp);
  Serial.print(",");
  Serial.print(huKi);
  Serial.print(",");
  Serial.print(huKd);
  Serial.print(",");
  Serial.print(deKp);
  Serial.print(",");
  Serial.print(deKi);
  Serial.print(",");
  Serial.println(deKd);
  Serial.print("Program,");
  Serial.print("Step,");
  Serial.print("Temp,");
  Serial.print("RH,");
  Serial.print("Temp_Setpoint,");
  Serial.print("RH_Setpoint,");
  Serial.print("heOutput,");
  Serial.print("huOutput,");
  Serial.print("coOutput,");
  Serial.println("deOutput");
 
  Alarm.timerRepeat(15, serialPrint);   //Prints statistics to Serial every 15 secs
  //Alarm.timerRepeat(61, timeSave);    //saves current run time for resume on power loss. In seconds

    programme = 0; //force boot to goto menu. Replace with EEPROMex
    prgStep = 0;  //

  heater.SetSampleTime(1000);  //PID time between calculations in ms
  cooler.SetSampleTime(1000);
  humidifier.SetSampleTime(1000);
  dehumidifier.SetSampleTime(1000);
  //write if first boot
  if (EEPROM.readDouble(48) != bootFlag){ //bootFlag is a version number, if new version, update stored values
    EEPROM.updateDouble(0,heKp);  //populate values from memory EXPERIMENTAL
    EEPROM.updateDouble(4,heKi);
    EEPROM.updateDouble(8,heKd);
    EEPROM.updateDouble(12,coKp);
    EEPROM.updateDouble(16,coKi);
    EEPROM.updateDouble(20,coKd);
    EEPROM.updateDouble(24,huKp);
    EEPROM.updateDouble(28,huKi);
    EEPROM.updateDouble(32,huKd);
    EEPROM.updateDouble(36,deKp);
    EEPROM.updateDouble(40,deKi);
    EEPROM.updateDouble(44,deKd);
    EEPROM.updateDouble(48,bootFlag);
    EEPROM.updateDouble(52,programme);
    EEPROM.updateDouble(56,prgStep);
    //EEPROM.updateDouble(60,runTimer);
    Serial.println("New version, Memory overwritten");    
  }
  heKp = EEPROM.readDouble(0);  //populate values from memory
  heKi = EEPROM.readDouble(4);  //Double occupies 4 bytes. Arduino Due board would require 8.
  heKd = EEPROM.readDouble(8);
  coKp = EEPROM.readDouble(12);
  coKi = EEPROM.readDouble(16);
  coKd = EEPROM.readDouble(20);
  huKp = EEPROM.readDouble(24);
  huKi = EEPROM.readDouble(28);
  huKd = EEPROM.readDouble(32);
  deKp = EEPROM.readDouble(36);
  deKi = EEPROM.readDouble(40);
  deKd = EEPROM.readDouble(44); // Memory to be allocated by something like this: double heKp = EEPROM.getAddress(sizeof(double));
  //bootflag in mem 48, no need to load again.
  programme = EEPROM.readDouble(52);
  prgStep = EEPROM.readDouble(56);
  //runTimer = EEPROM.readDouble(60);
  menuSetup();
    //
  menu.moveDown();
  switch (programme) {
    case 1:
    {
      prgStep--;
      menu.use();
      break;
    }
    case 2:
    {
      prgStep--;
      menu.moveDown();
      menu.use();
      break;
    }
    case 3:
    {
      prgStep--;
      menu.moveDown();
      menu.moveDown();
      menu.use();
      break;
    }
    case 4:
    {
      prgStep--;
      menu.moveDown();
      menu.moveDown();
      menu.moveDown();
      menu.use();
      break;
    }
    case 5:
    {
      prgStep--;
      menu.moveDown();
      menu.moveDown();
      menu.moveDown();
      menu.moveDown();
      menu.use();
      break;
    }
    case 6:
    {
      prgStep--;
      menu.moveDown();
      menu.moveDown();
      menu.moveDown();
      menu.moveDown();
      menu.moveDown();
      menu.use();
      break;
    }
    default :
    {
    break;
    }
  }
}

void loop() {
  process();
  // put your main code here, to run repeatedly: 
  //Serial.println("Flag 1 ");
  if (encoderValue > lastEncoded){
    if (calculating == false) menu.moveDown();
    lastEncoded = encoderValue;
  }
  else if (encoderValue < lastEncoded){
    if (calculating == false) menu.moveUp();
    lastEncoded = encoderValue;
  }
  else if (digitalRead(buttonPin) == LOW){
    if (calculating == false) {
      menu.use();
      menu.moveRight();
    }
    while (digitalRead(buttonPin) == LOW) delay(10);
  }
  else if (digitalRead(backButton) == LOW){
    if (calculating == false) menu.moveLeft();
    while (digitalRead(buttonPin) == LOW) delay(10);
  }
  else if (digitalRead(backButton) == LOW){
    calculating = false;
    menu.moveDown();
    programme = 0;
    prgStep = 0;
    prgSec = 0;
    prgMin = 0;
    prgHour = prgFive[prgStep];
    RH = prgFiveRH[prgStep];
    Temp = prgFiveTemp[prgStep];
    strcpy(name, "error");
    EEPROM.updateDouble(52,0);  //reset programme
    EEPROM.updateDouble(56,0);  //reset prgStep
    while (digitalRead(buttonPin) == LOW) delay(10);
  }
  Alarm.delay(10); // check if timer has expired
}
void doEncoderA(){
  // debounce
  if ( rotating ) delay (20);  // wait a little until the bouncing is done
    // Test transition, did things really change? 
  if( digitalRead(encoderPin1) != A_set ) {  // debounce once more
    A_set = !A_set;
      // adjust counter + if A leads B
    if ( A_set && !B_set )
      //lastEncoded = encoderValue;
      encoderValue += 1;
      rotating = false;  // no more debouncing until loop() hits again
  }
}

// Interrupt on B changing state, same as A above
void doEncoderB(){
  if ( rotating ) delay (20);
  if( digitalRead(encoderPin2) != B_set ) {
    B_set = !B_set;
    //  adjust counter - 1 if B leads A
    if( B_set && !A_set ) 
      //lastEncoded = encoderValue;
      encoderValue -= 1;
      rotating = false;
  }
}

void printscr()
{
  if (calculating == true) {
    time_t t = now()- runTimer;
    if(second() < 5){
      lcd.clear();
    }
    //Print run time information every seconds whilst calculating PID
    lcd.setCursor(0, 0);
    lcd.print("Temp:");
    lcd.print(doubleMap(averageTemp, 0, 1023, -45.2142, 80),2);
    lcd.setCursor(12, 0);
    lcd.print("RH:");
    lcd.print(doubleMap(averageRH, 0, 1023, -25.2142, 100),2);
    lcd.setCursor(0, 1);
    lcd.print("Out: ");
    lcd.print(Temp);
    lcd.setCursor(15, 1);
    lcd.print(RH);
    lcd.setCursor(0, 2);
    lcd.print("Prg:");
    lcd.print(name);
    lcd.setCursor(17, 2);
    lcd.print("S:");
    lcd.print(prgStep);
    lcd.setCursor(0, 3);
    lcd.print("Time:");
    lcd.setCursor(6, 3);
    lcd.print("H:");
    lcd.print(hour(t));
    lcd.setCursor(11, 3);
    lcd.print("M:");
    lcd.print(minute(t));
    lcd.setCursor(16, 3);
    lcd.print("S:");
    lcd.print(second(t));
    Alarm.timerOnce(1, printscr);
  }
}
  
void serialPrint()
{  //to be saved as a CSV file
  Serial.print(programme);
  Serial.print(",");
  Serial.print(prgStep);
  Serial.print(",");
  Serial.print(heInput);
  Serial.print(",");
  Serial.print(huInput);
  Serial.print(",");
  Serial.print(Temp);
  Serial.print(",");
  Serial.print(RH);
  Serial.print(",");
  Serial.print(heOutput);
  Serial.print(",");
  Serial.print(huOutput);
  Serial.print(",");
  Serial.print(coOutput);
  Serial.print(",");
  Serial.println(deOutput);
}
  
double doubleMap(double in, double A, double B, double C, double D) {
  double out;
  out = (in-A)/(B-A)*(D-C)+C;
  return out;
}  
  
void process() {
  totalTemp= totalTemp - readingsTemp[index];          
  readingsTemp[index] = analogRead(tempPin); 
  totalTemp= totalTemp + readingsTemp[index]; 
  totalRH= totalRH - readingsRH[index];          
  readingsRH[index] = analogRead(rhPin); 
  totalRH= totalRH + readingsRH[index];   
  index++;                    
  if (index >= numReadings)              
    index = 0;                           
  averageTemp = totalTemp / numReadings; 
  averageRH = totalRH / numReadings; 
  
  heInput = averageTemp;//map(averageTemp, 0, 1023, -45.2142, 80);
  coInput = heInput;
  huInput = averageRH;//map(averageRH, 0, 1023, -25.2142, 100);
  deInput = huInput;
  heater.Compute();
  cooler.Compute();
  humidifier.Compute();
  dehumidifier.Compute();
  unsigned long now = millis();
  if(now - windowStartTime>windowSize){ //time to shift the Relay Window
    windowStartTime += windowSize;
  }
  if(heOutput > now - windowStartTime){
    digitalWrite(hePin,HIGH); // 
  }
  else{
    digitalWrite(hePin,LOW); // 
  }
  analogWrite(coPin, coOutput); // 
  analogWrite(huPin, huOutput); // 
  analogWrite(dePin, deOutput); // 
}
  
void prg(){    //to exit program set step to 8 and call "prg();"
  if (prgStep == 0) {
    runTimer = now(); //- runTimer;
    EEPROM.updateDouble(52,programme);  //set programme selection to resume on power loss
    EEPROM.updateDouble(56,0);
    //setTime(0,0,0,1,1,13); // set time to Saturday 0:00:00am Jan 1 2013   // setTime to be synced to RTC
  }
  if(prgStep == 8){
    RH = 80;
    Temp = 10;
    prgStep = 0;
    programme = 0;
    //lcd.clear();
    EEPROM.updateDouble(52,0);  //reset programme memory to main menu
    EEPROM.updateDouble(56,0);  //reset program step to main menu
    //setTime(0,0,0,1,1,13); // set time to Saturday 0:00:00am Jan 1 2013   // setTime to be synced to RTC
    return;
  }
  prgStep++;
  EEPROM.updateDouble(56,prgStep); 
  Alarm.timerOnce(prgHour,prgMin,prgSec, prg);
  heSetpoint = doubleMap(Temp - deadband,-20,80,204.6,1023);  //1c deadband for temperature
  coSetpoint = doubleMap(Temp + deadband,-20,80,204.6,1023);
  huSetpoint = doubleMap(RH - deadband,0,100,204.6,1023);    //1% deadband for humidity
  deSetpoint = doubleMap(RH + deadband,0,100,204.6,1023);
}

void userInput(int menuFlag, float scale) { //Flag to interperate where its from, scale of scroll, defualt 1 = one scroll =+1, scale of 0.1 = one scroll =+0.1 etc.
  while (digitalRead(buttonPin) == LOW) delay(10);
  float numInput;
  boolean inputFlag = true;
  calculating = true;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("New Input: ");
  //encoderValue = 1;
  while (inputFlag == true) {
    if (encoderValue < 0 || encoderValue > 1000) encoderValue = 0;
    if (encoderValue != lastEncoded) {
      lcd.setCursor(0, 1);
      lcd.print("                   ");
      lcd.setCursor(0, 1);
      numInput = encoderValue * scale;
      lcd.print(numInput);
      lastEncoded = encoderValue;
    }
    //Read the user's input
    if (digitalRead(buttonPin) == LOW){
      inputFlag = false;
      lcd.setCursor(2, 0);
      switch (menuFlag) {  //case number, first digit: equipment type, second didgit: P,I or D
        case 11: 
          heKp = numInput;
          EEPROM.updateDouble(0,numInput);
          heater.SetTunings(numInput,heKi,heKd);// Heater Kp
          break;
        case 12: 
          heKi = numInput;
          EEPROM.updateDouble(4,numInput);
          heater.SetTunings(heKp,numInput,heKd);// Heater Ki
          break;
        case 13: 
          heKd = numInput;
          EEPROM.updateDouble(8,numInput);
          heater.SetTunings(heKp,heKi,numInput);// Heater Kd
          break;
        case 21: 
          coKp = numInput;
          EEPROM.updateDouble(12,numInput);
          cooler.SetTunings(numInput,coKi,coKd);// Cooler Kp
          break;
        case 22: 
          coKi = numInput;
          EEPROM.updateDouble(16,numInput);
          cooler.SetTunings(coKp,numInput,coKd);// Cooler Ki
          break;
        case 23: 
          coKd = numInput;
          EEPROM.updateDouble(20,numInput);
          cooler.SetTunings(coKp,coKi,numInput);// Cooler Kd
          break;
        case 31: 
          huKp = numInput;
          EEPROM.updateDouble(24,numInput);
          humidifier.SetTunings(numInput,huKi,huKd);//Humidifier Kp
          break;
        case 32: 
          huKi = numInput;
          EEPROM.updateDouble(28,numInput);
          humidifier.SetTunings(huKp,numInput,huKd);// Humidifier Ki
          break;
        case 33: 
          huKd = numInput;
          EEPROM.updateDouble(32,numInput);
          humidifier.SetTunings(huKp,huKi,numInput);// Humidifier Kd
          break;
        case 41: 
          deKp = numInput;
          EEPROM.updateDouble(36,numInput);
          dehumidifier.SetTunings(numInput,deKi,deKd);// Dehumidifier Kp
          break;
        case 42: 
          deKi = numInput;
          EEPROM.updateDouble(40,numInput);
          dehumidifier.SetTunings(deKp,numInput,deKd);// Dehumidifier Ki
          break;
        case 43: 
          deKd = numInput;
          EEPROM.updateDouble(44,numInput);//double update for double the fun.
          dehumidifier.SetTunings(deKp,deKi,numInput);// Dehumidifier Kd
          break;
         default:
           break;
        }
      }
    }
  calculating = false;
  while (digitalRead(buttonPin) == LOW) delay(10);
}
