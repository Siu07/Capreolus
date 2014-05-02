//01/04/14
//Version 0.0.7
//https://github.com/Siu07/Capreolus/tree/master/Capreolus
/* ______     ___      .______   .______    _______   ______    __       __    __       _______.
  /      |   /   \     |   _  \  |   _  \  |   ____| /  __  \  |  |     |  |  |  |     /       |
 |  ,----'  /  ^  \    |  |_)  | |  |_)  | |  |__   |  |  |  | |  |     |  |  |  |    |   (----`
 |  |      /  /_\  \   |   ___/  |      /  |   __|  |  |  |  | |  |     |  |  |  |     \   \    
 |  `----./  _____  \  |  |      |  |\  \-.|  |____ |  `--'  | |  `----.|  `--'  | .----)   |   
  \______/__/     \__\ | _|      | _| `.__||_______| \______/  |_______| \______/  |_______/    
                                                                                                   

      _______  __  .__   __.  _______     _______   ______     ______    _______       _______.
     |   ____||  | |  \ |  | |   ____|   |   ____| /  __  \   /  __  \  |       \     /       |
     |  |__   |  | |   \|  | |  |__      |  |__   |  |  |  | |  |  |  | |  .--.  |   |   (----`
     |   __|  |  | |  . `  | |   __|     |   __|  |  |  |  | |  |  |  | |  |  |  |    \   \    
     |  |     |  | |  |\   | |  |____    |  |     |  `--'  | |  `--'  | |  '--'  |.----)   |   
     |__|     |__| |__| \__| |_______|   |__|      \______/   \______/  |_______/ |_______/    */
                                                                                               
#include <MenuBackend.h>         //Menu element
#include <LiquidCrystal.h>       //LCD 20x4
#include <PID_v1.h>              //
#include <PID_AutoTune_v0.h>   Hope to integrate, but not yet.
#include <EEPROMex.h>            //Saving double varibles to EEPROM
#include <Time.h>                //Counting from boot time
#include <TimeAlarms.h>          //Basic multithreading
#include <Bounce2.h>             //Cleaning User inputs
#include <math.h>                //mapping numbers between differing scales

boolean encoderChris = true;  //my encoder or davids. Make False before publish
//Display
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
//Menu system
int programme =0, menuFlag = 0, numInput = 0;
//EEPROM
const int memBase = 350;
//PID other varibles
int RH, Temp;
int prgStep, prgHour, prgMin;
double deadband = 0.01; //set the deadband to prevent PID controllers fighting.
int hours = 0, mins =0, secs = 0;
int buttonState = 0;
long t = 0;
//Averageing inputs Temperature
const int numReadings = 10;
double readingsTemp[numReadings];      // the readings from the analog input
int index = 0;                  // the index of the current reading
double totalTemp = 0;                  // the running total
double averageTemp = 0;                // the average
//RH
double readingsRH[numReadings];      // the readings from the analog input
double totalRH = 0;                  // the running total
double averageRH = 0;                // the average
//IO - Input
int tempPin = A0;    //Define input pins
int rhPin = A1;
//User Input
int lastDebounceTime = 0;  // the last time the output pin was toggled
int debounceDelay = 50;    // the debounce time; increase if the output flickers
int storedPrg = 0;
//rotary encoder
enum PinAssignments {
  encoderPin1 = 2,   // rigth
  encoderPin2 = 3,   // left
  buttonPin = 20,    // another two pins
  backButton = 22
};
volatile unsigned int encoderValue = 0;  // a counter for the dial
volatile int lastEncoded = 0;
unsigned int lastReportedPos = 1;   // change management
static boolean rotating=false;      // debounce management
int oldValue = 0;
long lastencoderValue = 0;
// interrupt service routine vars
boolean A_set = false;              
boolean B_set = false;
//encoder sum, terrible name i know
int sum = 0;      
//heater           Define Default PID parameters            //need to set all to zero and read values from EEPROM
double heInput, heOutput, heSetpoint;      //suggestions for next attempt      //on boot
double heKp = 2, heKi = 1, heKd = 5;       //heKp = 2, heKi = 0.05, heKd = 5;
int windowSize = 10000;
unsigned long windowStartTime;
//cooler 
double coInput, coOutput, coSetpoint;
double coKp = 0.2, coKi = 0.1, coKd = 2;  //coKp = 0.2, coKi = 0.01, coKd = 2;
//humidifier
double huInput, huOutput, huSetpoint;
double huKp = 0.02, huKi = 0.01, huKd = 5;  //huKp = 0.02, huKi = 0.005, huKd = 5;
//dehumidifier
double deInput, deOutput, deSetpoint;
double deKp = 0.02, deKi = 0.01, deKd = 5;  //deKp = 0.02, deKi = 0.01, deKd = 5;
//Declaration of PID
PID heater(&heInput, &heOutput, &heSetpoint, heKp, heKi, heKd, DIRECT);
PID cooler(&coInput, &coOutput, &coSetpoint, coKp, coKi, coKd, REVERSE);
PID humidifier(&huInput, &huOutput, &huSetpoint, huKp, huKi, huKd, DIRECT);
PID dehumidifier(&deInput, &deOutput, &deSetpoint, deKp, deKi, deKd, REVERSE);
//IO - output
int hePin = 40;     //Define output pins
int coPin = 8;
int huPin = 9;
int dePin = 10;
//Programme One - Snack Salami          Eventually, array shall not be confined to 8 steps
int prgOne[8] = {2, 36, 6, 6, 6, 6, 36, 144}; // Hours
int prgOneRH[8] = {85, 92, 88, 85, 82, 80, 80, 80}; // Relative Humidity
int prgOneTemp[8] = {22, 25, 22, 20, 17, 14, 12, 10}; // Temperature
// Programme Two - Saucison Sec
int prgTwo[8] = {2, 48, 6, 6, 6, 12, 72, 144}; // Hours
int prgTwoRH[8] = {85, 92, 88, 85, 82, 80, 80, 80}; // Relative Humidity
int prgTwoTemp[8] = {22, 25, 22, 20, 17, 14, 12, 10}; // Temperature
// Programme three - Bacon
int prgThree[8] = {2, 12, 3, 3, 3, 3, 3, 9}; // Hours
int prgThreeRH[8] = {85, 92, 88, 85, 82, 80, 80, 80}; // Relative Humidity
int prgThreeTemp[8] = {22, 25, 22, 20, 17, 14, 12, 10}; // Temperature
// Programme Four - Large Meat
int prgFour[8] = {2, 60, 12, 12, 12, 12, 48, 132}; // Hours
int prgFourRH[8] = {85, 92, 88, 85, 82, 80, 80, 80}; // Relative Humidity
int prgFourTemp[8] = {22, 25, 22, 20, 17, 14, 12, 10}; // Temperature
//Calibration Program
int prgFive[8] = {59, 59, 59, 59, 59, 59, 59, 59}; // Seconds
int prgFiveRH[8] = {80, 80, 80, 80, 80, 80, 80, 80}; // Relative Humidity
int prgFiveTemp[8] = {20, 20, 20, 20, 20, 20, 20, 20}; // Temperature 
//Menu  Elements
MenuBackend menu = MenuBackend(menuUseEvent,menuChangeEvent);
  MenuItem selectPrg = MenuItem("Select Program");
    MenuItem prgMenu = MenuItem("Program");
      MenuItem runPrg = MenuItem("Program Run");
      MenuItem editPrg = MenuItem("Edit Program");
        MenuItem prgName = MenuItem("Program Name");
          MenuItem prgMenuStep = MenuItem("Program Steps");
            MenuItem prgSet = MenuItem("Program Setpoints");
      MenuItem delPrg = MenuItem("Delete Program");
        MenuItem delPrgConf = MenuItem("Delete Confirmation");
  MenuItem settings = MenuItem("Settings");
    MenuItem heat = MenuItem("Heater");
      MenuItem P = MenuItem("Proportional");
      MenuItem I = MenuItem("Integral");
      MenuItem D = MenuItem("Derivative");
    MenuItem cool = MenuItem("Cooler");
    MenuItem humid = MenuItem("Humidifier");
    MenuItem dehumid = MenuItem("Dehumidifier");
  MenuItem stat = MenuItem("Status");
//AutoTube parameters
byte ATuneModeRemember=2;
double aTuneStep=50, aTuneNoise=1, aTuneStartValue=100;  //output step for square wave 0-255, 
unsigned int aTuneLookBack=20;

boolean tuning = false;
unsigned long  modelTime, serialTime;

//set to false to connect to the real world
boolean useSimulation = true;
  
void setup()
{
  setTime(0,0,0,1,1,14);
  Serial.begin(9600);
  lcd.begin(20, 4);
  lcd.setCursor(0, 0);
  menuSetup();
  Serial.println("Starting navigation:\r\nUp: w   Down: s   Left: a   Right: d   Use: e");
  menu.moveDown();
  heater.SetOutputLimits(0, windowSize);
  heater.SetMode(AUTOMATIC); //Should be AUTOMATIC, only manual during testing
  cooler.SetMode(AUTOMATIC);
  humidifier.SetMode(AUTOMATIC);
  dehumidifier.SetMode(AUTOMATIC);
  windowStartTime = millis();
  EEPROM.setMemPool(memBase, EEPROMSizeMega); //minimum and maximum address to use. To be used with address allocation.
                                              //Min address should be set to the last manual known entry.
                                                //Repopulate PID values from memory. Double occupies 4 bytes. Arduino Due board would require 8.
  pinMode(A0, INPUT);     //Temperature input
  pinMode(A1, INPUT);     //RH input
  pinMode(A2, INPUT);     //Calibration Input
  pinMode(40, OUTPUT);    //Heater output
  pinMode(8, OUTPUT);     //Cooler Output
  pinMode(9, OUTPUT);     //Humidifier Output
  pinMode(10, OUTPUT);    //Dehumidifier Output
  heater.SetSampleTime(1000);
  cooler.SetSampleTime(1000);
  humidifier.SetSampleTime(1000);
  dehumidifier.SetSampleTime(1000);
  heKp = EEPROM.readDouble(0);
  heKi = EEPROM.readDouble(4);
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
  //User I/O
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

  for (int thisReading = 0; thisReading < numReadings; thisReading++)  //First Temperature and RH readings
    readingsTemp[thisReading] = 0;  
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readingsRH[thisReading] = 0;
  heSetpoint = 10;    //Set PID to aim for default behavior whilst awaiting programme selection
  huSetpoint = 80;    //Add to settings as an option rather than hardcode
}

void loop()
{
  if (Serial.available()) {
    byte read = Serial.read();
    switch (read) {
    case 'w': 
      menu.moveUp(); 
      break;
    case 's': 
      menu.moveDown(); 
      break;
    case 'd': 
      menu.moveRight(); 
      break;
    case 'a': 
      menu.moveLeft(); 
      break;
    case 'e': 
      menu.use(); 
      break;
    }
  }
  if (encoderValue > lastEncoded){
    menu.moveDown();
    lastEncoded = encoderValue;
  }
  else if (encoderValue < lastEncoded){
    menu.moveUp();
    lastEncoded = encoderValue;
  }
  else if (digitalRead(buttonPin) == LOW){
    menu.use();
    while (digitalRead(buttonPin) == LOW){
      delay(10);
    }
  }
  else if (digitalRead(backButton) == HIGH){  //put button wrong way around
    menu.moveLeft();
    while (digitalRead(buttonPin) == HIGH){
      delay(10);
    }
  }
  if(!programme == 0) {
    process();
  }
  Alarm.delay(10);  //non zero to avoid bug with libary
}

void userInput() {//use interrupt from encoder to call userInput(), change this to a flag when the select button is pushed.
  //lcd.clear();    //need to send it back here
  //lcd.setCursor(0, 0);
  //lcd.print("Enter a Value");
  lcd.setCursor(1, 0);
  lcd.print("New Input: ");
  while (digitalRead(buttonPin) == HIGH || Serial.available() < 1){
    lcd.setCursor(1, 12);
    lcd.print("         ");
    lcd.setCursor(1, 12);
    lcd.print(encoderValue);
    //Read the user's input
    if (digitalRead(buttonPin) == LOW || Serial.available() > 0){
      if (Serial.available() > 0)
        numInput = Serial.parseInt();
        else
          numInput = encoderValue;
      lcd.setCursor(2, 0);
      switch (menuFlag) {  //case number, first digit: equipment type, second didgit: P,I or D
        case 11: 
          Serial.println(heKp);
          heKp = EEPROM.updateDouble(0,numInput);
          lcd.print("Heater Kp: "); lcd.print(heKp);
          heater.SetTunings(numInput,heKi,heKd);// Heater Kp
          menuFlag = menuFlag - 1;
          Serial.print("Heater Kp changed to: ");    //optimise by putting repeated text in varible
          break;
        case 12: 
          Serial.println(heKi);
          heKi = EEPROM.updateDouble(4,numInput);
          lcd.print("Heater Ki: "); lcd.print(heKi);
          heater.SetTunings(heKp,numInput,heKd);// Heater Ki
          menuFlag = menuFlag - 2;
          Serial.print("Heater Ki changed to: ");
          break;
        case 13: 
          Serial.println(heKd);
          heKd = EEPROM.updateDouble(8,numInput);
          lcd.print("Heater Kd: "); lcd.print(heKd);
          heater.SetTunings(heKp,heKi,numInput);// Heater Kd
          menuFlag = menuFlag - 3;
          Serial.print("Heater Kd changed to: ");
          break;
        case 21: 
          Serial.println(coKp);
          coKp = EEPROM.updateDouble(12,numInput);
          lcd.print("Cooler Kp: "); lcd.print(coKp);
          cooler.SetTunings(numInput,coKi,coKd);// Cooler Kp
          menuFlag = menuFlag - 1;
          Serial.print("Cooler Kp changed to: ");
          break;
        case 22: 
          Serial.println(coKi);
          coKi = EEPROM.updateDouble(16,numInput);
          lcd.print("Cooler Ki: "); lcd.print(coKi);
          cooler.SetTunings(coKp,numInput,coKd);// Cooler Ki
          menuFlag = menuFlag - 2;
          Serial.print("Cooler Ki changed to: ");
          break;
        case 23: 
          Serial.println(coKd);
          coKd = EEPROM.updateDouble(20,numInput);
          lcd.print("Cooler Kd: "); lcd.print(coKd);
          cooler.SetTunings(coKp,coKi,numInput);// Cooler Kd
          menuFlag = menuFlag - 3;
          Serial.print("Cooler Kd changed to: ");
          break;
        case 31: 
          Serial.println(huKp);
          huKp = EEPROM.updateDouble(24,numInput);
          lcd.print("Humid Kp: "); lcd.print(huKp);
          humidifier.SetTunings(numInput,huKi,huKd);//Humidifier Kp
          menuFlag = menuFlag - 1;
          Serial.print("Humidifier Kp changed to: ");
          break;
        case 32: 
          Serial.println(huKi);
          huKi = EEPROM.updateDouble(28,numInput);
          lcd.print("Humid Ki: "); lcd.print(huKi);
          humidifier.SetTunings(huKp,numInput,huKd);// Humidifier Ki
          menuFlag = menuFlag - 2;
          Serial.print("Humidifier Ki changed to: ");
          break;
        case 33: 
          Serial.println(huKd);
          huKd = EEPROM.updateDouble(32,numInput);
          lcd.print("Humid Kd: "); lcd.print(huKd);
          humidifier.SetTunings(huKp,huKi,numInput);// Humidifier Kd
          menuFlag = menuFlag - 3;
          Serial.print("Humidifier Kd changed to: ");
          break;
        case 41: 
          Serial.println(deKp);
          deKp = EEPROM.updateDouble(36,numInput);
          lcd.print("DeHumid Kp: "); lcd.print(deKp);
          dehumidifier.SetTunings(numInput,deKi,deKd);// Dehumidifier Kp
          menuFlag = menuFlag - 1;
          Serial.print("Dehumidifier Kp changed to: ");
          break;
        case 42: 
          Serial.println(deKi);
          deKi = EEPROM.updateDouble(40,numInput);
          lcd.print("DeHumid Ki: "); lcd.print(deKi);
          dehumidifier.SetTunings(deKp,numInput,deKd);// Dehumidifier Ki
          menuFlag = menuFlag - 2;
          Serial.print("Dehumidifier Ki changed to: ");
          break;
        case 43: 
          Serial.println(deKd);
          deKd = EEPROM.updateDouble(44,numInput);//double update for double the fun.
          lcd.print("DeHumid Kd: "); lcd.print(deKd);
          dehumidifier.SetTunings(deKp,deKi,numInput);// Dehumidifier Kd
          menuFlag = menuFlag - 3;
          Serial.print("Dehumidifier Kd changed to: ");
          break;
         default:
           break;
           //Serial.println("Something went wrong!");
           //No input this loop
      }
      //Serial.println(numInput);
    }
    if(!programme == 0)  //keep calculating output whilst waiting for user selection
        process();
    // if(backButton == HIGH)  //Break out of loop if back button is pushed
    //    break;
  }
}

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
      //lastEncoded = encoded; //store this value for next time
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
        //lastEncoded = encoderValue;
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
        //lastEncoded = encoderValue;
        encoderValue -= 1;
        rotating = false;
    }
  }
}

double doubleMap(double in, double A, double B, double C, double D) {  //Converts values between different scales
  double out;                                                          //doubleMap(Input,original min, original max, new min, new max);
  out = (in-A)/(B-A)*(D-C)+C;                                          //example: input 5 in range 0-100, want range 0-500.
  return out;                                                          //doubleMap(5,0,100,0,500);
}                                                                      //output = (5-0)/(100-0)*(500-0)+0 = 25
