//-------------------------------------------------------------------
//
// Sous Vide Controller
// Bill Earl - for Adafruit Industries
// Edited by C.Davies
//
// Based on the Arduino PID and PID AutoTune Libraries 
// by Brett Beauregard
//------------------------------------------------------------------

// PID Library
#include <PID_v1.h>
#include <PID_AutoTune_v0.h>
#include <LiquidCrystal.h>       //LCD 20x4

// Libraries for the DS18B20 Temperature Sensor
#include <OneWire.h>
#include <DallasTemperature.h>

// So we can save and retrieve settings
#include <EEPROM.h>

// ************************************************
// Pin definitions
// ************************************************

// Output Relay
#define RelayPin 21

// One-Wire Temperature Sensor
#define ONE_WIRE_BUS 20

// interrupt service routine vars
boolean A_set = false;              
boolean B_set = false;

// Rotary Encoder vars
volatile unsigned int encoderValue = 0;  // a counter for the dial
volatile int lastEncoded = 0; //Encoder sum

enum PinAssignments {
  encoderPin1 = 3,   // scroll right
  encoderPin2 = 2,   // scroll left
  buttonPin = 5,    // Select
  backButton = 6    // back
};

static boolean rotating=false;      // debounce management

// ************************************************
// PID Variables and constants
// ************************************************

//Define Variables we'll be connecting to
double Setpoint;
double Input;
double Output;

volatile long onTime = 0;

// pid tuning parameters
double Kp;
double Ki;
double Kd;

// EEPROM addresses for persisted data
const int SpAddress = 0;
const int KpAddress = 8;
const int KiAddress = 16;
const int KdAddress = 24;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

// 10 second Time Proportional Output window
int WindowSize = 10000; 
unsigned long windowStartTime;

// ************************************************
// Auto Tune Variables and constants
// ************************************************
byte ATuneModeRemember=2;

double aTuneStep=500;
double aTuneNoise=1;
unsigned int aTuneLookBack=20;

boolean tuning = false;

PID_ATune aTune(&Input, &Output);

// ************************************************
// Display Variables and constants
// ************************************************

LiquidCrystal lcd(10, 16, 14, 15, 18, 19);

unsigned long lastInput = 0; // last button press

byte degree[8] = // define the degree symbol 
{ 
 B00110, 
 B01001, 
 B01001, 
 B00110, 
 B00000,
 B00000, 
 B00000, 
 B00000 
}; 

const int logInterval = 10000; // log every 10 seconds
long lastLogTime = 0;

// ************************************************
// States for state machine
// ************************************************
enum operatingState { OFF = 0, SETP, RUN, TUNE_P, TUNE_I, TUNE_D, AUTO};
operatingState opState = OFF;


// ************************************************
// Sensor Variables and constants
// Data wire is plugged into port 2 on the Arduino

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device address
DeviceAddress tempSensor;

// ************************************************
// Setup and diSplay initial screen
// ************************************************
void setup()
{
   Serial.begin(9600);

   // Initialize Relay Control:
   pinMode(RelayPin, OUTPUT);    // Output mode to drive relay
   digitalWrite(RelayPin, LOW);  // make sure it is off to start
   
  //boolean calculating = false;
  
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
  
   // Initialize LCD DiSplay 
   lcd.begin(16, 4);
   lcd.createChar(1, degree); // create degree symbol from the binary
   

   lcd.print(F("    Home"));
   lcd.setCursor(0, 1);
   lcd.print(F("   Sous Vide!"));

   // Start up the DS18B20 One Wire Temperature Sensor
   sensors.begin();
   if (!sensors.getAddress(tempSensor, 0)) 
   {
      lcd.setCursor(0, 1);
      lcd.print(F("Sensor Error"));
   }
   sensors.setResolution(tempSensor, 12);
   sensors.setWaitForConversion(false);

   delay(3000);  // Splash screen

   // Initialize the PID and related variables
   LoadParameters();
   myPID.SetTunings(Kp,Ki,Kd);

   myPID.SetSampleTime(1000);
   myPID.SetOutputLimits(0, WindowSize);
   opState = OFF;
}

// ************************************************
// Main Control Loop
//
// All state changes pass through here
// ************************************************
void loop()
{
   // wait for button release before changing state
   
  /* need some sort of user input here to select what the pid is doing*/
  
   lcd.clear();
   switch (opState)
   {
   case OFF:
      Off();
      break;
   case SETP:
      Tune_Sp();
      break;
    case RUN:
      Run();
      break;
   case TUNE_P:
      TuneP();
      break;
   case TUNE_I:
      TuneI();
      break;
   case TUNE_D:
      TuneD();
      break;
   }
}
// ************************************************
// Start the Auto-Tuning cycle
// ************************************************

void StartAutoTune()
{
   // Remember the mode we were in
   ATuneModeRemember = myPID.GetMode();

   // set up the auto-tune parameters
   aTune.SetNoiseBand(aTuneNoise);
   aTune.SetOutputStep(aTuneStep);
   aTune.SetLookbackSec((int)aTuneLookBack);
   tuning = true;
}
// ************************************************
// Save any parameter changes to EEPROM
// ************************************************
void SaveParameters()
{
   if (Setpoint != EEPROM_readDouble(SpAddress))
   {
      EEPROM_writeDouble(SpAddress, Setpoint);
   }
   if (Kp != EEPROM_readDouble(KpAddress))
   {
      EEPROM_writeDouble(KpAddress, Kp);
   }
   if (Ki != EEPROM_readDouble(KiAddress))
   {
      EEPROM_writeDouble(KiAddress, Ki);
   }
   if (Kd != EEPROM_readDouble(KdAddress))
   {
      EEPROM_writeDouble(KdAddress, Kd);
   }
}
// ************************************************
// Write floating point values to EEPROM
// ************************************************
void EEPROM_writeDouble(int address, double value)
{
   byte* p = (byte*)(void*)&value;
   for (int i = 0; i < sizeof(value); i++)
   {
      EEPROM.write(address++, *p++);
   }
}

// ************************************************
// Read floating point values from EEPROM
// ************************************************
double EEPROM_readDouble(int address)
{
   double value = 0.0;
   byte* p = (byte*)(void*)&value;
   for (int i = 0; i < sizeof(value); i++)
   {
      *p++ = EEPROM.read(address++);
   }
   return value;
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

// ************************************************
// Load parameters from EEPROM
// ************************************************
void LoadParameters()
{
  // Load from EEPROM
   Setpoint = EEPROM_readDouble(SpAddress);
   Kp = EEPROM_readDouble(KpAddress);
   Ki = EEPROM_readDouble(KiAddress);
   Kd = EEPROM_readDouble(KdAddress);
   
   // Use defaults if EEPROM values are invalid
   if (isnan(Setpoint))
   {
     Setpoint = 60;
   }
   if (isnan(Kp))
   {
     Kp = 850;
   }
   if (isnan(Ki))
   {
     Ki = 0.5;
   }
   if (isnan(Kd))
   {
     Kd = 0.1;
   }  
}

void userInput(int menuFlag, float scale) { //Flag to interperate where its from, scale of scroll, defualt 1 = one scroll =+1, scale of 0.1 = one scroll =+0.1 etc.
  while (digitalRead(buttonPin) == LOW) delay(10);
  float numInput;
  boolean inputFlag = true;
  //calculating = true;
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
      case 1: 
        Setpoint = numInput;
        //EEPROM.updateDouble(0,numInput);
        //heater.SetTunings(numInput,heKi,heKd);// Heater Kp
        break;
      case 2: 
        Kp = numInput;
        //EEPROM.updateDouble(0,numInput);
        //heater.SetTunings(numInput,heKi,heKd);// Heater Kp
        break;
      case 3: 
        Ki = numInput;
        //EEPROM.updateDouble(0,numInput);
        //heater.SetTunings(numInput,heKi,heKd);// Heater Kp
        break;
      case 4: 
        Kd = numInput;
        //EEPROM.updateDouble(0,numInput);
        //heater.SetTunings(numInput,heKi,heKd);// Heater Kp
        break;
      default:
        break;
      }
    }
  }
  //calculating = false;
  while (digitalRead(buttonPin) == LOW) delay(10);
}
// ************************************************
// Return to normal control
// ************************************************
void FinishAutoTune()
{
   tuning = false;

   // Extract the auto-tune calculated parameters
   Kp = aTune.GetKp();
   Ki = aTune.GetKi();
   Kd = aTune.GetKd();

   // Re-tune the PID and revert to normal control mode
   myPID.SetTunings(Kp,Ki,Kd);
   myPID.SetMode(ATuneModeRemember);
   
   // Persist any changed parameters to EEPROM
   SaveParameters();
}

// ************************************************
// Execute the control loop
// ************************************************
void DoControl()
{
  // Read the input:
  if (sensors.isConversionAvailable(0))
  {
    Input = sensors.getTempC(tempSensor);
    sensors.requestTemperatures(); // prime the pump for the next one - but don't wait
  }
  
  if (tuning) // run the auto-tuner
  {
     if (aTune.Runtime()) // returns 'true' when done
     {
        FinishAutoTune();
     }
  }
  else // Execute control algorithm
  {
     myPID.Compute();
  }
  
  // Time Proportional relay state is updated regularly via timer interrupt.
  onTime = Output; 
}

// ************************************************
// Initial State - press RIGHT to enter setpoint
// ************************************************
void Off()
{
   myPID.SetMode(MANUAL);
   //lcd.setBacklight(0);
   digitalWrite(RelayPin, LOW);  // make sure it is off
   lcd.print(F("    Adafruit"));
   lcd.setCursor(0, 1);
   lcd.print(F("   Sous Vide!"));
   //uint8_t buttons = 0;
   while (digitalRead(backButton) == HIGH) delay(10);
   // Prepare to transition to the RUN state
   sensors.requestTemperatures(); // Start an asynchronous temperature reading

   //turn the PID on
   myPID.SetMode(AUTOMATIC);
   windowStartTime = millis();
   opState = RUN; // start control
}
// ************************************************
// Called by ISR every 15ms to drive the output
// ************************************************
void DriveOutput()
{  
  long now = millis();
  // Set the output
  // "on time" is proportional to the PID output
  if(now - windowStartTime>WindowSize)
  { //time to shift the Relay Window
     windowStartTime += WindowSize;
  }
  if((onTime > 100) && (onTime > (now - windowStartTime)))
  {
     digitalWrite(RelayPin,HIGH);
  }
  else
  {
     digitalWrite(RelayPin,LOW);
  }
}

// ************************************************
// Setpoint Entry State
// ************************************************
void Tune_Sp()
{
    lcd.print(F("Set Temperature:"));
    userInput(1, 0.5);
    //if ((millis() - lastInput) > 3000)  // return to RUN after 3 seconds idle
    //{
       opState = RUN;
       return;
    //}
    lcd.setCursor(0,1);
    lcd.print(Setpoint);
    lcd.print(" ");
    DoControl();
    DriveOutput();
}

// ************************************************
// Proportional Tuning State
// ************************************************
void TuneP()
{
    lcd.print(F("Set Kp"));
    userInput(2, 10);
    //if ((millis() - lastInput) > 3000)  // return to RUN after 3 seconds idle
    //{
       opState = RUN;
       return;
    //}
    lcd.setCursor(0,1);
    lcd.print(Kp);
    lcd.print(" ");
    DoControl();
    DriveOutput();
}

// ************************************************
// Integral Tuning State
// ************************************************
void TuneI()
{
    lcd.print(F("Set Ki"));
    userInput(3, 0.1);
    //if ((millis() - lastInput) > 3000)  // return to RUN after 3 seconds idle
    //{
       opState = RUN;
       return;
    //}
    lcd.setCursor(0,1);
    lcd.print(Ki);
    lcd.print(" ");
    DoControl();
    DriveOutput();
}

// ************************************************
// Derivative Tuning State
// ************************************************
void TuneD()
{
    lcd.print(F("Set Kd"));
    userInput(4, 0.1);
    //if ((millis() - lastInput) > 3000)  // return to RUN after 3 seconds idle
    //{
       opState = RUN;
       return;
    //}
    lcd.setCursor(0,1);
    lcd.print(Kd);
    lcd.print(" ");
    DoControl();
    DriveOutput();
}


// ************************************************
// PID COntrol State
// SHIFT and RIGHT for autotune
// RIGHT - Setpoint
// LEFT - OFF
// ************************************************
void Run()
{
   // set up the LCD's number of rows and columns: 
   lcd.print(F("Sp: "));
   lcd.print(Setpoint);
   lcd.write(1);
   lcd.print(F("C : "));

   SaveParameters();
   myPID.SetTunings(Kp,Ki,Kd);

   uint8_t buttons = 0;
   while(true)
   {
      //setBacklight();  // set backlight based on state
      if ((digitalRead(buttonPin) == LOW) && (digitalRead(backButton) == LOW) && (abs(Input - Setpoint) < 0.5)){  // Should be at steady-state
      StartAutoTune();
      }
      else if (digitalRead(buttonPin) == LOW){
        opState = SETP;
        return;
      }
      else if (digitalRead(backButton) == LOW){
        opState = OFF;
        return;
      }
    }
      
      DoControl();
      DriveOutput();
      
      lcd.setCursor(0,1);
      lcd.print(Input);
      lcd.write(1);
      lcd.print(F("C : "));
      
      float pct = map(Output, 0, WindowSize, 0, 1000);
      lcd.setCursor(10,1);
      lcd.print(F("      "));
      lcd.setCursor(10,1);
      lcd.print(pct/10);
      //lcd.print(Output);
      lcd.print("%");

      lcd.setCursor(15,0);
      if (tuning)
      {
        lcd.print("T");
      }
      else
      {
        lcd.print(" ");
      }
      
      // periodically log to serial port in csv format
      if (millis() - lastLogTime > logInterval)  
      {
        Serial.print(Input);
        Serial.print(",");
        Serial.println(Output);
      }

    delay(100);
 }
