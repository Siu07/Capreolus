
//https://github.com/Siu07/Capreolus/tree/master/Capreolus
/*
todo:
 Done 100%: average last 5 seconds of reading to give consistant result
 Done 100%: make user input possible                                 Note: Buy a better rotary encoder, consider a keypad matrix
 auto turn off backlight                                             Note: add timeout, think about pwm to fade screen out.
 upgrade power supply                                                Note: 1A should be able to keep 12v steady. Need to get 2.1mm socket
 Done 75%: make routines more efficient                              Note: Using TimeAlarms to provide screen updates at regular intervals rather than slow down processing code.
 Done 100%:add real menu                                             Note: Experiment with M2tklib more. Probably require complete restructure of code.
 integrate data logging
 integrate phone support
 Done 100%: only map input reading for display, use unmapped for calculations
 Done 90%: resume on power loss.                                     Note: Rudimentory systems, No run time continuation, starts from begining of last recorded step.
 Done 100%: test "doubleMap" function.
 Done 100%: Note: Add "first boot" flag (with version number) to repopulate all EEPROM data on update or first run. 
 Done 0%: Add autotune function on its own menu
 Done 0%: Remove all charcutire related names, make everything generic and allow ability to change names of PID controllers and programs
 Done 0%: Allow number of programs and number of steps to be adjustable
 Done 0%: Add simple bang bang control when outside of close control (+-2c?) add menu option to overide PID.
 */
 /*
 EEPROM Memory Allocation:
  heKp         = 0      Heater
  heKi         = 4
  heKd         = 8
  coKp         = 12    Cooler
  coKi         = 16
  coKd         = 20
  huKp         = 24    Humidifier
  huKi         = 28
  huKd         = 32
  deKp         = 36    Dehumidifier
  deKi         = 40
  deKd         = 44
  bootFlag     = 48    first run of new version flag
  programme    = 52    Resume program on power loss
  prgStep      = 56    Resume step on power loss
  runTimer     = 60    Program timer
  prgOne       = 64
  prgTwo       = 80
  prgThree     = 96
  prgFour      = 112
  prgFive      = 128
  prgOneRH     = 144
  prgTwoRH     = 160
  prgThreeRH   = 176
  prgFourRH    = 192
  prgFiveRH    = 208
  prgOneTemp   = 224
  prgTwoTemp   = 240
  prgThreeTemp = 256
  prgFourTemp  = 272
  prgFiveTemp  = 288
 */
#include <MenuBackend.h>         //Menu element. version 1.4
#include <LiquidCrystal.h>       //LCD 20x4
#include <PID_v1.h>              //
//#include <PID_AutoTune_v0.h>  // Hope to integrate, but not yet.
#include <EEPROMex.h>            //Saving double varibles to EEPROM
#include <Time.h>                //Counting from boot time
#include <TimeAlarms.h>          //Basic multithreading
#include <Bounce2.h>             //Cleaning User inputs
#include <math.h>                //mapping numbers between differing scales

char Version[] = "28/12/15";
//boolean encoderChris = true;  //my encoder or davids. Make False before publish   //(What did i do with this code????)
double bootFlag = 0;  //version number, increment if PID values are changed
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
double TBand = 384.6;  //(+-2c) Temperature's +- this value from the target will switch control over to bang bang control
double HBand = 245.52;  //(+-5%)Humidities +- this value from the target will switch control over to bang bang control
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
void menuChangeEvent(MenuChangeEvent changed);
void menuUseEvent(MenuUseEvent used);
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
      MenuItem hourSets1 = MenuItem("Salami Hours");
        MenuItem hourSets11 = MenuItem("Salami Step 1 Hours");
        MenuItem hourSets12 = MenuItem("Salami Step 2 Hours");
        MenuItem hourSets13 = MenuItem("Salami Step 3 Hours");
        MenuItem hourSets14 = MenuItem("Salami Step 4 Hours");
        MenuItem hourSets15 = MenuItem("Salami Step 5 Hours");
        MenuItem hourSets16 = MenuItem("Salami Step 6 Hours");
        MenuItem hourSets17 = MenuItem("Salami Step 7 Hours");
        MenuItem hourSets18 = MenuItem("Salami Step 8 Hours");
        //Time settings
      MenuItem RHSets1 = MenuItem("Salami RH");
        MenuItem RHSets11 = MenuItem("Salami Step 1 RH");
        MenuItem RHSets12 = MenuItem("Salami Step 2 RH");
        MenuItem RHSets13 = MenuItem("Salami Step 3 RH");
        MenuItem RHSets14 = MenuItem("Salami Step 4 RH");
        MenuItem RHSets15 = MenuItem("Salami Step 5 RH");
        MenuItem RHSets16 = MenuItem("Salami Step 6 RH");
        MenuItem RHSets17 = MenuItem("Salami Step 7 RH");
        MenuItem RHSets18 = MenuItem("Salami Step 8 RH");
        //humidity settings
      MenuItem TempSets1 = MenuItem("Salami Temperature");
        MenuItem TempSets11 = MenuItem("Salami Step 1 Temp");
        MenuItem TempSets12 = MenuItem("Salami Step 2 Temp");
        MenuItem TempSets13 = MenuItem("Salami Step 3 Temp");
        MenuItem TempSets14 = MenuItem("Salami Step 4 Temp");
        MenuItem TempSets15 = MenuItem("Salami Step 5 Temp");
        MenuItem TempSets16 = MenuItem("Salami Step 6 Temp");
        MenuItem TempSets17 = MenuItem("Salami Step 7 Temp");
        MenuItem TempSets18 = MenuItem("Salami Step 8 Temp");
        //temperature settings
    MenuItem SetssaucionSec = MenuItem("Saucion Sec ");
      MenuItem hourSets2 = MenuItem("Saucion Hours");
        MenuItem hourSets21 = MenuItem("Saucion Step 1 Hours");
        MenuItem hourSets22 = MenuItem("Saucion Step 2 Hours");
        MenuItem hourSets23 = MenuItem("Saucion Step 3 Hours");
        MenuItem hourSets24 = MenuItem("Saucion Step 4 Hours");
        MenuItem hourSets25 = MenuItem("Saucion Step 5 Hours");
        MenuItem hourSets26 = MenuItem("Saucion Step 6 Hours");
        MenuItem hourSets27 = MenuItem("Saucion Step 7 Hours");
        MenuItem hourSets28 = MenuItem("Saucion Step 8 Hours");
        //Time settings
      MenuItem RHSets2 = MenuItem("Saucion Sec RH");
        MenuItem RHSets21 = MenuItem("Saucion Step 1 RH");
        MenuItem RHSets22 = MenuItem("Saucion Step 2 RH");
        MenuItem RHSets23 = MenuItem("Saucion Step 3 RH");
        MenuItem RHSets24 = MenuItem("Saucion Step 4 RH");
        MenuItem RHSets25 = MenuItem("Saucion Step 5 RH");
        MenuItem RHSets26 = MenuItem("Saucion Step 6 RH");
        MenuItem RHSets27 = MenuItem("Saucion Step 7 RH");
        MenuItem RHSets28 = MenuItem("Saucion Step 8 RH");
        //humidity settings
      MenuItem TempSets2 = MenuItem("Saucion Sec Temperature");
        MenuItem TempSets21 = MenuItem("Saucion Step 1 Temp");
        MenuItem TempSets22 = MenuItem("Saucion Step 2 Temp");
        MenuItem TempSets23 = MenuItem("Saucion Step 3 Temp");
        MenuItem TempSets24 = MenuItem("Saucion Step 4 Temp");
        MenuItem TempSets25 = MenuItem("Saucion Step 5 Temp");
        MenuItem TempSets26 = MenuItem("Saucion Step 6 Temp");
        MenuItem TempSets27 = MenuItem("Saucion Step 7 Temp");
        MenuItem TempSets28 = MenuItem("Saucion Step 8 Temp");
        //temperature settings
    MenuItem Setsbacon = MenuItem("Bacon ");
      MenuItem hourSets3 = MenuItem("Bacon Hours");
        MenuItem hourSets31 = MenuItem("Bacon Step 1 Hours");
        MenuItem hourSets32 = MenuItem("Bacon Step 2 Hours");
        MenuItem hourSets33 = MenuItem("Bacon Step 3 Hours");
        MenuItem hourSets34 = MenuItem("Bacon Step 4 Hours");
        MenuItem hourSets35 = MenuItem("Bacon Step 5 Hours");
        MenuItem hourSets36 = MenuItem("Bacon Step 6 Hours");
        MenuItem hourSets37 = MenuItem("Bacon Step 7 Hours");
        MenuItem hourSets38 = MenuItem("Bacon Step 8 Hours");
        //Time settings
      MenuItem RHSets3 = MenuItem("Bacon RH");
        MenuItem RHSets31 = MenuItem("Bacon Step 1 RH");
        MenuItem RHSets32 = MenuItem("Bacon Step 2 RH");
        MenuItem RHSets33 = MenuItem("Bacon Step 3 RH");
        MenuItem RHSets34 = MenuItem("Bacon Step 4 RH");
        MenuItem RHSets35 = MenuItem("Bacon Step 5 RH");
        MenuItem RHSets36 = MenuItem("Bacon Step 6 RH");
        MenuItem RHSets37 = MenuItem("Bacon Step 7 RH");
        MenuItem RHSets38 = MenuItem("Bacon Step 8 RH");
        //humidity settings
      MenuItem TempSets3 = MenuItem("Bacon Temperature");
        MenuItem TempSets31 = MenuItem("Bacon Step 1 Temp");
        MenuItem TempSets32 = MenuItem("Bacon Step 2 Temp");
        MenuItem TempSets33 = MenuItem("Bacon Step 3 Temp");
        MenuItem TempSets34 = MenuItem("Bacon Step 4 Temp");
        MenuItem TempSets35 = MenuItem("Bacon Step 5 Temp");
        MenuItem TempSets36 = MenuItem("Bacon Step 6 Temp");
        MenuItem TempSets37 = MenuItem("Bacon Step 7 Temp");
        MenuItem TempSets38 = MenuItem("Bacon Step 8 Temp");
        //temperature settings
    MenuItem SetslargeMeat = MenuItem("Large Meat ");
      MenuItem hourSets4 = MenuItem("Large Meat Hours");
        MenuItem hourSets41 = MenuItem("Large Step 1 Hours");
        MenuItem hourSets42 = MenuItem("Large Step 2 Hours");
        MenuItem hourSets43 = MenuItem("Large Step 3 Hours");
        MenuItem hourSets44 = MenuItem("Large Step 4 Hours");
        MenuItem hourSets45 = MenuItem("Large Step 5 Hours");
        MenuItem hourSets46 = MenuItem("Large Step 6 Hours");
        MenuItem hourSets47 = MenuItem("Large Step 7 Hours");
        MenuItem hourSets48 = MenuItem("Large Step 8 Hours");
        //Time settings
      MenuItem RHSets4 = MenuItem("Large Meat RH");
        MenuItem RHSets41 = MenuItem("Large Step 1 RH");
        MenuItem RHSets42 = MenuItem("Large Step 2 RH");
        MenuItem RHSets43 = MenuItem("Large Step 3 RH");
        MenuItem RHSets44 = MenuItem("Large Step 4 RH");
        MenuItem RHSets45 = MenuItem("Large Step 5 RH");
        MenuItem RHSets46 = MenuItem("Large Step 6 RH");
        MenuItem RHSets47 = MenuItem("Large Step 7 RH");
        MenuItem RHSets48 = MenuItem("Large Step 8 RH");
        //humidity settings
      MenuItem TempSets4 = MenuItem("Large Meat Temperature");
        MenuItem TempSets41 = MenuItem("Large Step 1 Temp");
        MenuItem TempSets42 = MenuItem("Large Step 2 Temp");
        MenuItem TempSets43 = MenuItem("Large Step 3 Temp");
        MenuItem TempSets44 = MenuItem("Large Step 4 Temp");
        MenuItem TempSets45 = MenuItem("Large Step 5 Temp");
        MenuItem TempSets46 = MenuItem("Large Step 6 Temp");
        MenuItem TempSets47 = MenuItem("Large Step 7 Temp");
        MenuItem TempSets48 = MenuItem("Large Step 8 Temp");
        //temperature settings
    MenuItem SetsHolding = MenuItem("Holding ");
      MenuItem hourSets5 = MenuItem("Holding Hours");
        MenuItem hourSets51 = MenuItem("Holding Step 1 Hours");
        MenuItem hourSets52 = MenuItem("Holding Step 2 Hours");
        MenuItem hourSets53 = MenuItem("Holding Step 3 Hours");
        MenuItem hourSets54 = MenuItem("Holding Step 4 Hours");
        MenuItem hourSets55 = MenuItem("Holding Step 5 Hours");
        MenuItem hourSets56 = MenuItem("Holding Step 6 Hours");
        MenuItem hourSets57 = MenuItem("Holding Step 7 Hours");
        MenuItem hourSets58 = MenuItem("Holding Step 8 Hours");
        //Time settings
      MenuItem RHSets5 = MenuItem("Holding RH");
        MenuItem RHSets51 = MenuItem("Holding Step 1 RH");
        MenuItem RHSets52 = MenuItem("Holding Step 2 RH");
        MenuItem RHSets53 = MenuItem("Holding Step 3 RH");
        MenuItem RHSets54 = MenuItem("Holding Step 4 RH");
        MenuItem RHSets55 = MenuItem("Holding Step 5 RH");
        MenuItem RHSets56 = MenuItem("Holding Step 6 RH");
        MenuItem RHSets57 = MenuItem("Holding Step 7 RH");
        MenuItem RHSets58 = MenuItem("Holding Step 8 RH");
        //humidity settings
      MenuItem TempSets5 = MenuItem("Holding Temperature");
        MenuItem TempSets51 = MenuItem("Holding Step 1 Temp");
        MenuItem TempSets52 = MenuItem("Holding Step 2 Temp");
        MenuItem TempSets53 = MenuItem("Holding Step 3 Temp");
        MenuItem TempSets54 = MenuItem("Holding Step 4 Temp");
        MenuItem TempSets55 = MenuItem("Holding Step 5 Temp");
        MenuItem TempSets56 = MenuItem("Holding Step 6 Temp");
        MenuItem TempSets57 = MenuItem("Holding Step 7 Temp");
        MenuItem TempSets58 = MenuItem("Holding Step 8 Temp");
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
  //lcd.print(F("Booting")); //Pointless as it boots so fast
  EEPROM.setMemPool(memBase, EEPROMSizeMega); //minimum and maximum address to use. To be used with address allocation.
  EEPROM.setMaxAllowedWrites(1024);  //must be greater than the total number of addresses to be written to on new version first boot
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
    //Serial.println(F("Save as a .CSV file"));
  Serial.println(F("Date,HeaterKp,HeaterKi,HeaterKd,CoolerKp,CoolerKi,CoolerKd,HumididiferKp,HumididiferKi,HumididiferKd,DehumidifierKp,DehumidifierKi,DehumidifierKd"));
  Serial.print(F("27/12/15,"));
  Serial.print(heKp);
  Serial.print(F(","));
  Serial.print(heKi);
  Serial.print(F(","));
  Serial.print(heKd);
  Serial.print(F(","));
  Serial.print(coKp);
  Serial.print(F(","));
  Serial.print(coKi);
  Serial.print(F(","));
  Serial.print(coKd);
  Serial.print(F(","));
  Serial.print(huKp);
  Serial.print(F(","));
  Serial.print(huKi);
  Serial.print(F(","));
  Serial.print(huKd);
  Serial.print(F(","));
  Serial.print(deKp);
  Serial.print(F(","));
  Serial.print(deKi);
  Serial.print(F(","));
  Serial.println(deKd);
  Serial.print(F("Program,"));
  Serial.print(F("Step,"));
  Serial.print(F("Temp,"));
  Serial.print(F("RH,"));
  Serial.print(F("Temp_Setpoint,"));
  Serial.print(F("RH_Setpoint,"));
  Serial.print(F("heOutput,"));
  Serial.print(F("huOutput,"));
  Serial.print(F("coOutput,"));
  Serial.println(F("deOutput"));
  Alarm.timerRepeat(15, serialPrint);   //Prints statistics to Serial every 15 secs
  //Alarm.timerRepeat(61, timeSave);    //saves current run time for resume on power loss. In seconds
  programme = 0; //force boot to goto menu. Replace with EEPROMex
  prgStep = 0;  //
  heater.SetSampleTime(10000);  //PID time between calculations in ms
  cooler.SetSampleTime(10000);
  humidifier.SetSampleTime(10000);
  dehumidifier.SetSampleTime(10000);
  //write if first boot
  if (EEPROM.readDouble(48) != bootFlag){ //bootFlag is a version number, if new version, update stored values
    EEPROM.updateDouble(0,heKp);  //populate values from defaults
    EEPROM.updateDouble(4,heKi);  //double = 4 byte
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
    EEPROM.updateBlock<int>(64, prgOne, 8);    //int = 2 byte
    EEPROM.updateBlock<int>(80, prgTwo, 8);
    EEPROM.updateBlock<int>(96, prgThree, 8);
    EEPROM.updateBlock<int>(112, prgFour, 8);
    EEPROM.updateBlock<int>(128, prgFive, 8);
    EEPROM.updateBlock<int>(144, prgOneRH, 8);
    EEPROM.updateBlock<int>(160, prgTwoRH, 8);
    EEPROM.updateBlock<int>(176, prgThreeRH, 8);
    EEPROM.updateBlock<int>(192, prgFourRH, 8);
    EEPROM.updateBlock<int>(208, prgFiveRH, 8);
    EEPROM.updateBlock<int>(224, prgOneTemp, 8);
    EEPROM.updateBlock<int>(240, prgTwoTemp, 8);
    EEPROM.updateBlock<int>(256, prgThreeTemp, 8);
    EEPROM.updateBlock<int>(272, prgFourTemp, 8);
    EEPROM.updateBlock<int>(288, prgFiveTemp, 8);
    //EEPROM.updateDouble(60,runTimer);
    Serial.println(F("New version, Memory overwritten"));    
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
  deKd = EEPROM.readDouble(44);
  EEPROM.readBlock<int>(64, prgOne, 8);
  EEPROM.readBlock<int>(80, prgTwo, 8);
  EEPROM.readBlock<int>(96, prgThree, 8);
  EEPROM.readBlock<int>(112, prgFour, 8);
  EEPROM.readBlock<int>(128, prgFive, 8);
  EEPROM.readBlock<int>(144, prgOneRH, 8);
  EEPROM.readBlock<int>(160, prgTwoRH, 8);
  EEPROM.readBlock<int>(176, prgThreeRH, 8);
  EEPROM.readBlock<int>(192, prgFourRH, 8);
  EEPROM.readBlock<int>(208, prgFiveRH, 8);
  EEPROM.readBlock<int>(224, prgOneTemp, 8);
  EEPROM.readBlock<int>(240, prgTwoTemp, 8);
  EEPROM.readBlock<int>(256, prgThreeTemp, 8);
  EEPROM.readBlock<int>(272, prgFourTemp, 8);
  EEPROM.readBlock<int>(288, prgFiveTemp, 8); // Memory to be allocated by something like this: double heKp = EEPROM.getAddress(sizeof(double));
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
  Alarm.delay(10); // check if timer has expired
  // put your main code here, to run repeatedly: 
  //Serial.println(F("Flag 1 "));
  if (encoderValue > lastEncoded){
    if (calculating == false) menu.moveDown();
    lastEncoded = encoderValue;
  }
  else if (encoderValue < lastEncoded){
    if (calculating == false) menu.moveUp();
    lastEncoded = encoderValue;
  }
  else if (digitalRead(buttonPin) == LOW){
    while (digitalRead(buttonPin) == LOW) delay(10);
    if (calculating == false) {
      menu.use();
      menu.moveRight();
    }
  }
  else if (digitalRead(backButton) == LOW){
    while (digitalRead(backButton) == LOW) delay(10);
    if (calculating == false) menu.moveLeft();
    else {
      //Serial.println(F("Flag 4"));
    Serial.println(calculating);
      calculating = false;
      lcd.clear();
      menu.moveBack();
      programme = 0;
      prgSec = 0;
      prgMin = 0;
      prgHour = prgFive[prgStep];
      RH = prgFiveRH[prgStep];
      Temp = prgFiveTemp[prgStep];
      strcpy(name, "error");
      EEPROM.updateDouble(52,0);  //reset programme
      EEPROM.updateDouble(56,0);  //reset prgStep
    }
    prgStep = 0;
  }
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
    //if(second() < 5){
    //  lcd.clear();
    //}
    //Print run time information every seconds whilst calculating PID
    lcd.setCursor(0, 0);
    lcd.print(F("Temp:"));
    lcd.print(doubleMap(averageTemp, 0, 1023, -45.2142, 80),2);
    lcd.setCursor(12, 0);
    lcd.print(F("RH:"));
    lcd.print(doubleMap(averageRH, 0, 1023, -25.2142, 100),2);
    lcd.setCursor(0, 1);
    lcd.print(F("Out: "));
    lcd.print(Temp);
    lcd.setCursor(15, 1);
    lcd.print(RH);
    lcd.setCursor(0, 2);
    lcd.print(F("Prg:"));
    lcd.print(name);
    lcd.setCursor(17, 2);
    lcd.print(F("S:"));
    lcd.print(prgStep);
    lcd.setCursor(0, 3);
    lcd.print(F("Time:"));
    lcd.setCursor(6, 3);
    lcd.print(F("H:"));
    lcd.print(hour(t));
    lcd.setCursor(11, 3);
    lcd.print(F("M:"));
    lcd.print(minute(t));
    lcd.setCursor(16, 3);
    lcd.print(F("S:"));
    lcd.print(second(t));
    Alarm.timerOnce(1, printscr);
  }
}
  
void serialPrint()
{  //to be saved as a CSV file
  Serial.print(programme);
  Serial.print(F(","));
  Serial.print(prgStep);
  Serial.print(F(","));
  Serial.print(heInput);
  Serial.print(F(","));
  Serial.print(huInput);
  Serial.print(F(","));
  Serial.print(Temp);
  Serial.print(F(","));
  Serial.print(RH);
  Serial.print(F(","));
  Serial.print(heOutput);
  Serial.print(F(","));
  Serial.print(huOutput);
  Serial.print(F(","));
  Serial.print(coOutput);
  Serial.print(F(","));
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
  
  heInput = averageTemp;//map(averageTemp, 0, 1023, -45.2142, 80); no need to map
  coInput = heInput;
  huInput = averageRH;//map(averageRH, 0, 1023, -25.2142, 100);
  deInput = huInput;

  if(heInput >= heSetpoint + TBand){  //if outside of close control, use bang bang control
      heOutput = 0;
      coOutput = 1023;
  }
  else if(heInput <= heSetpoint - TBand){
      heOutput = 1023;
      coOutput = 0;
  }
  else{
    heater.Compute();
    cooler.Compute();
  }
  if(huInput >= huSetpoint + HBand){
      huOutput = 0;
      deOutput = 1023;
  }
  else if(huInput <= huSetpoint - HBand){
      huOutput = 1023;
      deOutput = 0;
  }
  else{
    humidifier.Compute();
    dehumidifier.Compute();
  }

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
    runTimer = now();
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
  lcd.print(F("New Input: "));
  //encoderValue = 1;
  while (inputFlag == true) {
    if (encoderValue < 0 || encoderValue > 1000) encoderValue = 0;
    if (encoderValue != lastEncoded) {
      lcd.setCursor(0, 1);
      lcd.print(F("                   "));
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
      case 51://hourSets11//hours 1
        prgOne[0] = numInput; 
        EEPROM.update(64, numInput);
        break;//hourSets12
      case 52:
        prgOne[1] = numInput; 
        EEPROM.update(66, numInput);
        break;//hourSets13
      case 53:
        prgOne[2] = numInput; 
        EEPROM.update(68, numInput);
        break;//hourSets14
      case 54:
        prgOne[3] = numInput; 
        EEPROM.update(70, numInput);
        break;//hourSets15
      case 55:
        prgOne[4] = numInput; 
        EEPROM.update(72, numInput);
        break;//hourSets16
      case 56:
        prgOne[5] = numInput; 
        EEPROM.update(74, numInput);
        break;//hourSets17
      case 57:
        prgOne[6] = numInput; 
        EEPROM.update(76, numInput);
        break;//hourSets18
      case 58:
        prgOne[7] = numInput; 
        EEPROM.update(78, numInput);
        break;//hourSets21//hours 2
      case 61:
        prgTwo[0] = numInput; 
        EEPROM.update(80, numInput);
        break;//hourSets22
      case 62:
        prgTwo[1] = numInput; 
        EEPROM.update(82, numInput);
        break;//hourSets23
      case 63:
        prgTwo[2] = numInput; 
        EEPROM.update(84, numInput);
        break;//hourSets24
      case 64:
        prgTwo[3] = numInput; 
        EEPROM.update(86, numInput);
        break;//hourSets25
      case 65:
        prgTwo[4] = numInput; 
        EEPROM.update(88, numInput);
        break;//hourSets26
      case 66:
        prgTwo[5] = numInput; 
        EEPROM.update(90, numInput);
        break;//hourSets27
      case 67:
        prgTwo[6] = numInput; 
        EEPROM.update(92, numInput);
        break;//hourSets28
      case 68:
        prgTwo[7] = numInput; 
        EEPROM.update(94, numInput);
        break;//hourSets31//hours 3
      case 71:
        prgThree[0] = numInput; 
        EEPROM.update(96, numInput);
        break;//hourSets32
      case 72:
        prgThree[1] = numInput; 
        EEPROM.update(98, numInput);
        break;//hourSets33
      case 73:
        prgThree[2] = numInput; 
        EEPROM.update(100, numInput);
        break;//hourSets34
      case 74:
        prgThree[3] = numInput; 
        EEPROM.update(102, numInput);
        break;//hourSets35
      case 75:
        prgThree[4] = numInput; 
        EEPROM.update(104, numInput);
        break;//hourSets36
      case 76:
        prgThree[5] = numInput; 
        EEPROM.update(106, numInput);
        break;//hourSets37
      case 77:
        prgThree[6] = numInput; 
        EEPROM.update(108, numInput);
        break;//hourSets38
      case 78:
        prgThree[7] = numInput; 
        EEPROM.update(110, numInput);
        break;//hourSets41//hours 4
      case 81:
        prgFour[0] = numInput; 
        EEPROM.update(112, numInput);
        break;//hourSets42
      case 82:
        prgFour[1] = numInput; 
        EEPROM.update(114, numInput);
        break;//hourSets43
      case 83:
        prgFour[2] = numInput; 
        EEPROM.update(116, numInput);
        break;//hourSets44
      case 84:
        prgFour[3] = numInput; 
        EEPROM.update(118, numInput);
        break;//hourSets45
      case 85:
        prgFour[4] = numInput; 
        EEPROM.update(120, numInput);
        break;//hourSets46
      case 86:
        prgFour[5] = numInput; 
        EEPROM.update(122, numInput);
        break;//hourSets47
      case 87:
        prgFour[6] = numInput; 
        EEPROM.update(124, numInput);
        break;//hourSets48
      case 88:
        prgFour[7] = numInput; 
        EEPROM.update(126, numInput);
        break;//hourSets51//hours 5
      case 91:
        prgFive[0] = numInput; 
        EEPROM.update(128, numInput);
        break;//hourSets52
      case 92:
        prgFive[1] = numInput; 
        EEPROM.update(130, numInput);
        break;//hourSets53
      case 93:
        prgFive[2] = numInput; 
        EEPROM.update(132, numInput);
        break;//hourSets54
      case 94:
        prgFive[3] = numInput; 
        EEPROM.update(134, numInput);
        break;//hourSets55
      case 95:
        prgFive[4] = numInput; 
        EEPROM.update(136, numInput);
        break;//hourSets56
      case 96:
        prgFive[5] = numInput; 
        EEPROM.update(138, numInput);
        break;//hourSets57
      case 97:
        prgFive[6] = numInput; 
        EEPROM.update(140, numInput);
        break;//hourSets58
      case 98:
        prgFive[7] = numInput; 
        EEPROM.update(142, numInput);
        break;//RHSets11//RH 1
      case 101:
        prgOneRH[0] = numInput; 
        EEPROM.update(144, numInput);
        break;//RHSets12
      case 102:
        prgOneRH[1] = numInput; 
        EEPROM.update(146, numInput);
        break;//RHSets13
      case 103:
        prgOneRH[2] = numInput; 
        EEPROM.update(148, numInput);
        break;//RHSets14
      case 104:
        prgOneRH[3] = numInput; 
        EEPROM.update(150, numInput);
        break;//RHSets15
      case 105:
        prgOneRH[4] = numInput; 
        EEPROM.update(152, numInput);
        break;//RHSets16
      case 106:
        prgOneRH[5] = numInput; 
        EEPROM.update(154, numInput);
        break;//RHSets17
      case 107:
        prgOneRH[6] = numInput; 
        EEPROM.update(156, numInput);
        break;//RHSets18
      case 108:
        prgOneRH[7] = numInput; 
        EEPROM.update(158, numInput);
        break;//RHSets21//RH 2
      case 111:
        prgTwoRH[0] = numInput; 
        EEPROM.update(160, numInput);
        break;//RHSets22
      case 112:
        prgTwoRH[1] = numInput; 
        EEPROM.update(162, numInput);
        break;//RHSets23
      case 113:
        prgTwoRH[2] = numInput; 
        EEPROM.update(164, numInput);
        break;//RHSets24
      case 114:
        prgTwoRH[3] = numInput; 
        EEPROM.update(166, numInput);
        break;//RHSets25
      case 115:
        prgTwoRH[4] = numInput; 
        EEPROM.update(168, numInput);
        break;//RHSets26
      case 116:
        prgTwoRH[5] = numInput; 
        EEPROM.update(170, numInput);
        break;//RHSets27
      case 117:
        prgTwoRH[6] = numInput; 
        EEPROM.update(172, numInput);
        break;//RHSets28
      case 118:
        prgTwoRH[7] = numInput; 
        EEPROM.update(174, numInput);
        break;//RHSets31//RH 3
      case 121:
        prgThreeRH[0] = numInput; 
        EEPROM.update(176, numInput);
        break;//RHSets32
      case 122:
        prgThreeRH[1] = numInput; 
        EEPROM.update(178, numInput);
        break;//RHSets33
      case 123:
        prgThreeRH[2] = numInput; 
        EEPROM.update(180, numInput);
        break;//RHSets34
      case 124:
        prgThreeRH[3] = numInput; 
        EEPROM.update(182, numInput);
        break;//RHSets35
      case 125:
        prgThreeRH[4] = numInput; 
        EEPROM.update(184, numInput);
        break;//RHSets36
      case 126:
        prgThreeRH[5] = numInput; 
        EEPROM.update(186, numInput);
        break;//RHSets37
      case 127:
        prgThreeRH[6] = numInput; 
        EEPROM.update(188, numInput);
        break;//RHSets38
      case 128:
        prgThreeRH[7] = numInput; 
        EEPROM.update(190, numInput);
        break;//RHSets41//RH 4
      case 131:
        prgFourRH[0] = numInput; 
        EEPROM.update(192, numInput);
        break;//RHSets42
      case 132:
        prgFourRH[1] = numInput; 
        EEPROM.update(194, numInput);
        break;//RHSets43
      case 133:
        prgFourRH[2] = numInput; 
        EEPROM.update(196, numInput);
        break;//RHSets44
      case 134:
        prgFourRH[3] = numInput; 
        EEPROM.update(198, numInput);
        break;//RHSets45
      case 135:
        prgFourRH[4] = numInput; 
        EEPROM.update(200, numInput);
        break;//RHSets46
      case 136:
        prgFourRH[5] = numInput; 
        EEPROM.update(202, numInput);
        break;//RHSets47
      case 137:
        prgFourRH[6] = numInput; 
        EEPROM.update(204, numInput);
        break;//RHSets48
      case 138:
        prgFourRH[7] = numInput; 
        EEPROM.update(206, numInput);
        break;//RHSets51//RH 5
      case 141:
        prgFiveRH[0] = numInput; 
        EEPROM.update(208, numInput);
        break;//RHSets52
      case 142:
        prgFiveRH[1] = numInput; 
        EEPROM.update(210, numInput);
        break;//RHSets53
      case 143:
        prgFiveRH[2] = numInput; 
        EEPROM.update(212, numInput);
        break;//RHSets54
      case 144:
        prgFiveRH[3] = numInput; 
        EEPROM.update(214, numInput);
        break;//RHSets55
      case 145:
        prgFiveRH[4] = numInput; 
        EEPROM.update(216, numInput);
        break;//RHSets56
      case 146:
        prgFiveRH[5] = numInput; 
        EEPROM.update(218, numInput);
        break;//RHSets57
      case 147:
        prgFiveRH[6] = numInput; 
        EEPROM.update(220, numInput);
        break;//RHSets58
      case 148:
        prgFiveRH[7] = numInput; 
        EEPROM.update(222, numInput);
        break;//TempSets11//Temps 1
      case 151:
        prgOneTemp[0] = numInput; 
        EEPROM.update(224, numInput);
        break;//TempSets12
      case 152:
        prgOneTemp[1] = numInput; 
        EEPROM.update(226, numInput);
        break;//TempSets13
      case 153:
        prgOneTemp[2] = numInput; 
        EEPROM.update(228, numInput);
        break;//TempSets14
      case 154:
        prgOneTemp[3] = numInput; 
        EEPROM.update(230, numInput);
        break;//TempSets15
      case 155:
        prgOneTemp[4] = numInput; 
        EEPROM.update(232, numInput);
        break;//TempSets16
      case 156:
        prgOneTemp[5] = numInput; 
        EEPROM.update(234, numInput);
        break;//TempSets17
      case 157:
        prgOneTemp[6] = numInput; 
        EEPROM.update(236, numInput);
        break;//TempSets18
      case 158:
        prgOneTemp[7] = numInput; 
        EEPROM.update(238, numInput);
        break;//TempSets21//Temps 2
      case 161:
        prgTwoTemp[0] = numInput; 
        EEPROM.update(240, numInput);
        break;//TempSets22
      case 162:
        prgTwoTemp[1] = numInput; 
        EEPROM.update(242, numInput);
        break;//TempSets23
      case 163:
        prgTwoTemp[2] = numInput; 
        EEPROM.update(244, numInput);
        break;//TempSets24
      case 164:
        prgTwoTemp[3] = numInput; 
        EEPROM.update(246, numInput);
        break;//TempSets25
      case 165:
        prgTwoTemp[4] = numInput; 
        EEPROM.update(248, numInput);
        break;//TempSets26
      case 166:
        prgTwoTemp[5] = numInput; 
        EEPROM.update(250, numInput);
        break;//TempSets27
      case 167:
        prgTwoTemp[6] = numInput; 
        EEPROM.update(252, numInput);
        break;//TempSets28
      case 168:
        prgTwoTemp[7] = numInput; 
        EEPROM.update(254, numInput);
        break;//TempSets31//Temps 3
      case 171:
        prgThreeTemp[0] = numInput; 
        EEPROM.update(256, numInput);
        break;//TempSets32
      case 172:
        prgThreeTemp[1] = numInput; 
        EEPROM.update(258, numInput);
        break;//TempSets33
      case 173:
        prgThreeTemp[2] = numInput; 
        EEPROM.update(260, numInput);
        break;//TempSets34
      case 174:
        prgThreeTemp[3] = numInput; 
        EEPROM.update(262, numInput);
        break;//TempSets35
      case 175:
        prgThreeTemp[4] = numInput; 
        EEPROM.update(264, numInput);
        break;//TempSets36
      case 176:
        prgThreeTemp[5] = numInput; 
        EEPROM.update(266, numInput);
        break;//TempSets37
      case 177:
        prgThreeTemp[6] = numInput; 
        EEPROM.update(268, numInput);
        break;//TempSets38
      case 178:
        prgThreeTemp[7] = numInput; 
        EEPROM.update(270, numInput);
        break;//TempSets41//Temps 4
      case 181:
        prgFourTemp[0] = numInput; 
        EEPROM.update(272, numInput);
        break;//TempSets42
      case 182:
        prgFourTemp[1] = numInput; 
        EEPROM.update(274, numInput);
        break;//TempSets43
      case 183:
        prgFourTemp[2] = numInput; 
        EEPROM.update(276, numInput);
        break;//TempSets44
      case 184:
        prgFourTemp[3] = numInput; 
        EEPROM.update(278, numInput);
        break;//TempSets45
      case 185:
        prgFourTemp[4] = numInput; 
        EEPROM.update(280, numInput);
        break;//TempSets46
      case 186:
        prgFourTemp[5] = numInput; 
        EEPROM.update(282, numInput);
        break;//TempSets47
      case 187:
        prgFourTemp[6] = numInput; 
        EEPROM.update(284, numInput);
        break;//TempSets48
      case 188:
        prgFourTemp[7] = numInput; 
        EEPROM.update(286, numInput);
        break;//TempSets51//Temps 5
      case 191:
        prgFiveTemp[0] = numInput; 
        EEPROM.update(288, numInput);
        break;//TempSets52
      case 192:
        prgFiveTemp[1] = numInput; 
        EEPROM.update(290, numInput);
        break;//TempSets53
      case 193:
        prgFiveTemp[2] = numInput; 
        EEPROM.update(292, numInput);
        break;//TempSets54
      case 194:
        prgFiveTemp[3] = numInput; 
        EEPROM.update(294, numInput);
        break;//TempSets55
      case 195:
        prgFiveTemp[4] = numInput; 
        EEPROM.update(296, numInput);
        break;//TempSets56
      case 196:
        prgFiveTemp[5] = numInput; 
        EEPROM.update(298, numInput);
        break;//TempSets57
      case 197:
        prgFiveTemp[6] = numInput; 
        EEPROM.update(300, numInput);
        break;//TempSets58
      case 198:
        prgFiveTemp[7] = numInput; 
        EEPROM.update(302, numInput);
        break;
      default:
        break;
      }
    }
  }
  menu.moveDown();
  menu.moveUp();
  calculating = false;
  while (digitalRead(buttonPin) == LOW) delay(10);
}
