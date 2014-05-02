//Version 020414
//https://github.com/Siu07/Capreolus/tree/master/Capreolus
/*
todo:
 Done 100%: average last 5 seconds of reading to give consistant result
 make user input possible                                            Note: Buy a better rotary encoder, consider a keypad matrix
 auto turn off backlight                                             Note: add timeout, think about pwm to fade screen out.
 upgrade power supply                                                Note: 1A should be able to keep 12v steady. Need to get 2.1mm socket
 Done 75%: make routines more efficient                              Note: Using TimeAlarms to provide screen updates at regular intervals rather than slow down processing code.
 add real menu                                                       Note: Experiment with M2tklib more. Probably require complete restructure of code.
 integrate data logging
 integrate phone support
 only map input reading for display, use unmapped for calculations
 Done 50%: resume on power loss.                                     Note: Rudimentory systems, auto resumes previous program and step on power. No run time continuation, recorded step started from begining.
 add first boot mode to populate EEPROM.
test "doubleMap" function.
Note: Add "first boot" flag (with version number) to repopulate all EEPROM data on update or first run. 
 */
 /*
 EEPROM Memory Allocation:
 0-119 = programs
 0-23 = program 1
 24-47 = program 2
 48-71 = program 3
 72-95 = program 4
 96-119 = program 5
 
 120 = storedPrg
 121 = storedStep
 */
//#include <EEPROMAnything.h>
//#include <EEPROM.h>
#include <EEPROMex.h>            //Saving double varibles to EEPROM
#include <math.h> 
#include <PID_v1.h>
#include <Time.h>
#include <TimeAlarms.h>
#include <DS1307RTC.h>
#include <LiquidCrystal.h>
#include <Bounce.h>
#include <Wire.h>
//#include <phi_interfaces.h>
boolean encoderChris = true;  //my encoder or davids. Make False before publish
//rotary encoder
enum PinAssignments {
  encoderPin1 = 2,   // rigth
  encoderPin2 = 3,   // left
  buttonPin = 20    // another two pins
};
volatile unsigned int encoderValue = 0;  // a counter for the dial
unsigned int lastReportedPos = 1;   // change management
static boolean rotating=false;      // debounce management
// interrupt service routine vars
boolean A_set = false;              
boolean B_set = false;
//Display
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

//PID
int RH, Temp;
int programme, prgStep, prgHour, prgMin, prgSec;
double deadband = 0.5; //set the deadband to prevent PID controllers fighting.
int hours = 0, mins =0, secs = 0;
int buttonState = 0;
long t = 0;

//Averageing inputs
const int numReadings = 10;
double readingsTemp[numReadings];      // the readings from the analog input
int index = 0;                  // the index of the current reading
double totalTemp = 0;                  // the running total
double averageTemp = 0;                // the average

double readingsRH[numReadings];      // the readings from the analog input
double totalRH = 0;                  // the running total
double averageRH = 0;                // the average

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

boolean menuSelect = true;
boolean menuReturn = true;
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


int oldValue = 0;

volatile int lastEncoded = 0;

long lastencoderValue = 0;
int sum = 0;

long timer = 0;
long boot = 0;

//int prgOne[8]; // Hours
//int prgOneRH[8]; // Relative Humidity
//int prgOneTemp[8]; // Temperature
//// Programme Two - Saucison Sec
//int prgTwo[8]; // Hours
//int prgTwoRH[8]; // Relative Humidity
//int prgTwoTemp[8]; // Temperature
//// Programme three - Bacon
//int prgThree[8]; // Hours
//int prgThreeRH[8]; // Relative Humidity
//int prgThreeTemp[8]; // Temperature
//// Programme Four - Large Meat
//int prgFour[8]; // Hours
//int prgFourRH[8]; // Relative Humidity
//int prgFourTemp[8]; // Temperature
////Calibration Program
//int prgFive[8]; // Seconds
//int prgFiveRH[8]; // Relative Humidity
//int prgFiveTemp[8]; // Temperature

// To be changed to array of arrays in eeprom (3 arrays)

//heater                                      Define PID parameters
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

PID heater(&heInput, &heOutput, &heSetpoint, heKp, heKi, heKd, DIRECT);    //initilize PIDs
PID cooler(&coInput, &coOutput, &coSetpoint, coKp, coKi, coKd, REVERSE);
PID humidifier(&huInput, &huOutput, &huSetpoint, huKp, huKi, huKd, DIRECT);
PID dehumidifier(&deInput, &deOutput, &deSetpoint, deKp, deKi, deKd, REVERSE);

void setup()
{
  Serial.begin(9600);
  lcd.begin(20, 4);
  lcd.setCursor(0, 0);
  lcd.print("Booting"); //Pointless as it boots so fast
  int addr = 0;  //First address
  //  for (int i=0; i <= 7; i++){
  //    prgOne[i] = EEPROM.read(addr);
  //    addr++;
  //  }
  //  for (int i=0; i <= 7; i++){
  //    prgOneRH[i] = EEPROM.read(addr);
  //    addr++;
  //  }
  //  for (int i=0; i <= 7; i++){
  //    prgOneTemp[i] = EEPROM.read(addr);
  //    addr++;
  //  }
  //  //Fill 2
  //  for (int i=0; i <= 7; i++){
  //    prgTwo[i] = EEPROM.read(addr);
  //    addr++;
  //  }
  //  for (int i=0; i <= 7; i++){
  //    prgTwoRH[i] = EEPROM.read(addr);
  //    addr++;
  //  }
  //  for (int i=0; i <= 7; i++){
  //    prgTwoTemp[i] = EEPROM.read(addr);
  //    addr++;
  //  }
  //  //Fill 3
  //  for (int i=0; i <= 7; i++){
  //    prgThree[i] = EEPROM.read(addr);
  //    addr++;
  //  }
  //  for (int i=0; i <= 7; i++){
  //    prgThreeRH[i] = EEPROM.read(addr);
  //    addr++;
  //  }
  //  for (int i=0; i <= 7; i++){
  //    prgThreeTemp[i] = EEPROM.read(addr);
  //    addr++;
  //  }
  //  //Fill 4
  //  for (int i=0; i <= 7; i++){
  //    prgFour[i] = EEPROM.read(addr);
  //    addr++;
  //  }
  //  for (int i=0; i <= 7; i++){
  //    prgFourRH[i] = EEPROM.read(addr);
  //    addr++;
  //  }
  //  for (int i=0; i <= 7; i++){
  //    prgFourTemp[i] = EEPROM.read(addr);
  //    addr++;
  //  }
  //  //Fill 5
  //  for (int i=0; i <= 7; i++){
  //    prgFive[i] = EEPROM.read(addr);
  //    addr++;
  //  }
  //  for (int i=0; i <= 7; i++){
  //    prgFiveRH[i] = EEPROM.read(addr);
  //    addr++;
  //  }
  //  for (int i=0; i <= 7; i++){
  //    prgFiveTemp[i] = EEPROM.read(addr);
  //    addr++;
  //  }
  setTime(0,0,0,1,1,13); // set time to Saturday 0:00:00am Jan 1 2013
  // setTime to be synced to RTC
  // Select programme within loop, map to 1,2,3,4 or 5
  
  


  //pinMode(A0, INPUT);     //Temperature input
  //pinMode(A1, INPUT);     //RH input
  //pinMode(A2, INPUT);     //Calibration Input

  pinMode(40, OUTPUT);    //Heater output
  //pinMode(8, OUTPUT);     //Cooler Output
  //pinMode(9, OUTPUT);     //Humidifier Output
  //pinMode(10, OUTPUT);    //Dehumidifier Output
  
  //rotary encoder
  pinMode(encoderPin1, INPUT); 
  pinMode(encoderPin2, INPUT); 
  pinMode(buttonPin, INPUT);
 // turn on pullup resistors
  digitalWrite(encoderPin1, HIGH);
  digitalWrite(encoderPin2, HIGH);
  digitalWrite(buttonPin, HIGH);

// encoder pin on interrupt 0 (pin 2)
  attachInterrupt(0, doEncoderA, CHANGE);
// encoder pin on interrupt 1 (pin 3)
  attachInterrupt(1, doEncoderB, CHANGE);

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
  //first boot setup. Comment out until a flag can be put around to only do on version change.
  //EEPROM.write(120, 0);  //wipes last boot program
  //EEPROM.write(121, 0);  //wipes last boot program step
  Alarm.timerRepeat(15, serialPrint);
  //storedPrg = EEPROM.read(120);  //Resume on power loss
  //if(storedPrg == 0){
    programme = 0; //force boot to goto menu
    prgStep = 0;  //
  //}
  //else{
  //  programme = storedPrg; 
  //  prgStep = EEPROM.read(121) - 1; //Force it to go back one step so it can go through the Prg() function, write to lcd and up back up one step
 //   prg();
 // }
}    

void  loop(){
  process();
  //write statistics to the screen and monitor buttons for exit (with conformation)
  //feedback current step, current temp, target temp, current RH, target RH, time left total, time to next step
  Alarm.delay(10); // check if timer has expired
  
}

// interface to thermister for calibration run and to detect outside ambient
//float Thermister(int RawADC) {  
//  float tempIn;
//  tempIn = log(((4812800/RawADC) - 4700));
//  tempIn = 1 / (0.0011657006 + (0.0002508247 + (0.0000001077 * tempIn * tempIn ))* tempIn );
//  tempIn = tempIn - 273.15;            // Convert Kelvin to Celcius
//  // Temp = (Temp * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
//  return tempIn;
//}

void exitB(){
  if ((millis() - lastDebounceTime) > debounceDelay) {  //debounce (shared)
    menuReturn = false;
    //do stuff here
    Serial.print("exit");
    lastDebounceTime = millis();
  }
}

void selectB(){
  if ((millis() - lastDebounceTime) > debounceDelay) {  //debounce (shared)
    menuSelect = false;
    Serial.print("select");
    //do stuff here
    lastDebounceTime = millis();
  }
}

void prg(){    //to exit program set step to 8 and call "prg();"
  if (prgStep == 0) {
    boot = now();
    EEPROM.write(120, programme);
    EEPROM.write(121, 0);
  }
  if(prgStep == 8){
    RH = 80;
    Temp = 10;
    programme = 0;// forced go to menu
    prgStep = 0;
    lcd.clear();
    EEPROM.write(120, 0);
    EEPROM.write(121, 0);    
    return;
  }
  else {
    switch (programme) {
    case 1:
      { // Programme One - Snack Salami
        prgSec = 0;
        prgMin = 0;
        prgHour = prgOne[prgStep];
        RH = prgOneRH[prgStep];
        Temp = prgOneTemp[prgStep];
        break;
      }
    case 2:
      { // Programme Two - Saucisson Sec
        prgSec = 0;
        prgMin = 0;
        prgHour = prgTwo[prgStep];
        RH = prgTwoRH[prgStep];
        Temp = prgTwoTemp[prgStep];
        break;
      }
    case 3:
      { // Programme three - Bacon
        prgSec = 0;
        prgMin = 0;
        prgHour = prgThree[prgStep];
        RH = prgThreeRH[prgStep];
        Temp = prgThreeTemp[prgStep];
        break;
      }
    case 4:
      { // Programme Four - Large Meat
        prgSec = 0;
        prgMin = prgFour[prgStep];
        prgHour = 0;
        //prgMin = 0;                    //ORIGINAL
        //prgHour = prgFour[prgStep];
        RH = prgFourRH[prgStep];
        Temp = prgFourTemp[prgStep];
        break;
      }
    case 5:
      { // Programme Five - Holding
        prgSec = 0;
        prgMin = 0;
        prgHour = prgFive[prgStep];
        RH = prgFiveRH[prgStep];
        Temp = prgFiveTemp[prgStep];
        break;
      }
    case 6:
      {
        //will contain code for user to input changes to programs
        encoderValue = 0;
        programme = 0; //force boot to go back to menu
        prgStep = 0;
        break;
      }
    case 7:  
      {
        //contains Information about the current version within a sub menu
        lcd.setCursor(0, 0);
        lcd.print("Version 02/04/14");
        encoderValue = 1;    //lands on first menu list item
        delay(500);  //Gives enough time for previous button press to rise before next press
        buttonState = digitalRead(buttonPin);
        while (buttonState == HIGH) {      //Holds on menu
          buttonState = digitalRead(buttonPin);
          if (encoderValue != oldValue){
            if(encoderValue > 4) {
              encoderValue = 1;
            }
            if(encoderValue < 1) {
              encoderValue = 4;
            }
            oldValue = encoderValue;
            lcd.setCursor(0, 1);
            lcd.print("                    ");
            lcd.setCursor(0, 3);
            lcd.print("                    ");
            lcd.setCursor(0, 1);
            switch (encoderValue) {
            case 1:
              { // Programme One - Snack Salami
                lcd.print("Heater");
                lcd.setCursor(0, 2);
                lcd.print("    P    I    D");
                lcd.setCursor(0, 3);
                lcd.print("  ");
                lcd.print(heKp);
                lcd.print(" ");
                lcd.print(heKi);
                lcd.print(" ");
                lcd.print(heKd);
                lcd.print(" ");
                break;
              }
            case 2:
              { // Programme Two - Saucison Sec
                lcd.print("Cooler");
                lcd.setCursor(0, 2);
                lcd.print("    P    I    D");
                lcd.setCursor(0, 3);
                lcd.print("  ");
                lcd.print(" ");
                lcd.print(coKp);
                lcd.print(" ");
                lcd.print(coKi);
                lcd.print(" ");
                lcd.print(coKd);
                lcd.print(" ");
                break;
              }
            case 3:
              { // Programme three - Bacon
                lcd.print("Humidifier");
                lcd.setCursor(0, 2);
                lcd.print("    P    I    D");
                lcd.setCursor(0, 3);
                lcd.print("  ");
                lcd.print(huKp);
                lcd.print(" ");
                lcd.print(huKi);
                lcd.print(" ");
                lcd.print(huKd);
                break;
              }
            case 4:
              { // Programme Four - Large Meat
                lcd.print("Dehumidifier");
                lcd.setCursor(0, 2);
                lcd.print("    P    I    D");
                lcd.setCursor(0, 3);
                lcd.print("  ");
                lcd.print(deKp);
                lcd.print(" ");
                lcd.print(deKi);
                lcd.print(" ");
                lcd.print(deKd);
                break;
              }  
            }
          }
        }
        encoderValue = 0;
        programme = 0; //force boot to go back to menu
        prgStep = 0;
        break;
      }
    default:
      {  //error handling
        return;
        break;
      }
    }
    prgStep++;
    EEPROM.write(121, prgStep);
    Alarm.timerOnce(prgHour,prgMin,prgSec, prg);

    heSetpoint = doubleMap(Temp - deadband,-20,80,204.6,1023);  //1c deadband for temperature
    coSetpoint = doubleMap(Temp + deadband,-20,80,204.6,1023);
    huSetpoint = doubleMap(RH - deadband,0,100,204.6,1023);    //1% deadband for humidity
    deSetpoint = doubleMap(RH + deadband,0,100,204.6,1023);

    lcd.setCursor(17, 2);
    lcd.print("S:");
    lcd.print(prgStep);
    lcd.setCursor(0, 0);
    lcd.print("Temp:");
    lcd.setCursor(12, 0);
    lcd.print("RH:");
    lcd.setCursor(0, 1);
    lcd.print("Out:");
    lcd.setCursor(0, 2);
    lcd.print("Prg:");
  }
}
//encoder fit in David's device
//not sure why i did it this way, should just take any two of the three, store which direction it last went and compare to next high signal to see if it went up or down relative
void updateEncoder(){
  if (encoderChris = false){
    if ((millis() - lastDebounceTime) > debounceDelay) {
      lastDebounceTime = millis();
      int MSB = digitalRead(encoderPin1); //MSB = most significant bit
      int LSB = digitalRead(encoderPin2); //LSB = least significant bit
      int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
      sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value
      if(sum == 13 || sum == 7) encoderValue ++;
      if(sum == 14 || sum == 11) encoderValue --;
      lastEncoded = encoded; //store this value for next time
    }
  }
}

void doEncoderA(){
  // debounce
  if (encoderChris = true){
    if ( rotating ) delay (1);  // wait a little until the bouncing is done
      // Test transition, did things really change? 
    if( digitalRead(encoderPin1) != A_set ) {  // debounce once more
      A_set = !A_set;
        // adjust counter + if A leads B
      if ( A_set && !B_set ) 
        encoderValue += 1;
        rotating = false;  // no more debouncing until loop() hits again
    }
  }
}

// Interrupt on B changing state, same as A above
void doEncoderB(){
  if (encoderChris = true){
    if ( rotating ) delay (1);
    if( digitalRead(encoderPin2) != B_set ) {
      B_set = !B_set;
      //  adjust counter - 1 if B leads A
      if( B_set && !A_set ) 
        encoderValue -= 1;
        rotating = false;
    }
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

void printscr()
{
  time_t t = now()- boot;
  if(second() < 10){
    lcd.setCursor(6, 3);
    lcd.print("              ");
  }
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
  
  if(programme != 0){
    lcd.setCursor(5, 0);
    lcd.print(doubleMap(averageTemp, 0, 1023, -45.2142, 80),2);//map(heInput, 0, 1023, -20, 80));
    lcd.setCursor(15, 0);
    lcd.print(doubleMap(averageRH, 0, 1023, -25.2142, 100),2);//map(huInput, 0, 1023, -20, 80));
    lcd.setCursor(5, 1);
    lcd.print(Temp);//doubleMap(heSetpoint, 204.6, 1023, -20, 80),2);
    lcd.setCursor(15, 1);
    lcd.print(RH);//doubleMap(huSetpoint, 204.6, 1023, 0, 100),2);

    lcd.setCursor(5, 2);
    switch (programme) {
    case 1:
      { // Programme One - Snack Salami
        lcd.print("Snack Salami");
        break;
      }
    case 2:
      { // Programme Two - Saucison Sec
        lcd.print("Saucisson Sec");
        break;
      }
    case 3:
      { // Programme three - Bacon
        lcd.print("Bacon");
        break;
      }
    case 4:
      { // Programme Four - Large Meat
        lcd.print("Test");
        break;
      }
    case 5:
      { // Programme Five - Calibration
        lcd.print("Holding");
        break;
      }
    case 6:
      { // Edit programs
        lcd.print("Edit Programs");
        break;
      }
    case 7:
      { // Version number
        lcd.print("Version Info");
        break;
      }
    }
  } 
  else {
    //user to select program
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Program Select:");
    time_t t = now()- boot;
    if(second() < 10){
      lcd.setCursor(6, 3);
      lcd.print("              ");
    }
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
    //time_t t = now()- boot;
    buttonState = digitalRead(buttonPin);
    while (buttonState == HIGH) {      //uncomment, testing only
      process();
      Alarm.delay(10);
      buttonState = digitalRead(buttonPin);
      if (encoderValue != oldValue){
        if(encoderValue > 7) {
          encoderValue = 1;
        }
        if(encoderValue < 1) {
          encoderValue = 7;
        }
        oldValue = encoderValue;
        lcd.setCursor(0, 2);
        lcd.print("                    ");
        lcd.setCursor(0, 2);
        switch (encoderValue) {
        case 1:
          { // Programme One - Snack Salami
            lcd.print("Snack Salami");
            break;
          }
        case 2:
          { // Programme Two - Saucison Sec
            lcd.print("Saucisson Sec");
            break;
          }
        case 3:
          { // Programme three - Bacon
            lcd.print("Bacon");
            break;
          }
        case 4:
          { // Programme Four - Large Meat
            lcd.print("Test");
            break;
          }
        case 5:
          { // Programme Five - Calibration
            lcd.print("Holding");
            break;
          }
        case 6:
          {  // Edit programs
            lcd.print("Edit Programs");
            break;
          }
        case 7:
          {  // Version number
            lcd.print("Version Info");
            break;
          }
        }
      }
    }
    menuSelect = true;  
    menuReturn = true;
    programme = encoderValue;
    lcd.clear();
    prg();
  }
  Alarm.timerOnce(1, printscr);
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

double doubleMap(double in, double A, double B, double C, double D) {
  double out;
  out = (in-A)/(B-A)*(D-C)+C;
  return out;
}
  
  
