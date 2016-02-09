#include <LiquidCrystal.h>       //LCD 20x4
#include <PID_v1.h>              //
//#include <Bounce2.h>             //Cleaning User inputs
#include <math.h>                //mapping numbers between differing scales
#include <DallasTemperature.h>   //Small temperature sensing module
#include <OneWire.h>
#include <Time.h>                //Counting from boot time
#include <TimeAlarms.h>          //Basic multithreading

enum PinAssignments {
  buttonPin = 20,    // Select
  backButton = 22    // back
};

//Display
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

//PID
int Temp;
int programme, prgStep, prgHour, prgMin, prgSec;
double deadband = 0.5; //set the deadband to prevent PID controllers fighting.
double TBand = 384.6;  //(+-2c) Temperature's +- this value from the target will switch control over to bang bang control
double tempTemp = 20;
double hePrint;
int hours = 0, mins =0, secs = 0;
int buttonState = 0;
long t = 0;

//Averageing inputs Temperature
const int numReadings = 10;
double readingsTemp[numReadings];      // the readings from the analog input
int index = 0;                  // the index of the current reading
double totalTemp = 0;                  // the running total
double averageTemp = 0;                // the average

//Arduino hardware IO
int tempPin = A0;    //Define input pins
int hePin = 14;     //Define output pins

//User Input
//int lastDebounceTime = 0;  // the last time the output pin was toggled
//int debounceDelay = 10;    // the debounce time; increase if the output flickers

long runTimer = 0;    //milli's since program start

//heater                                      Define default PID parameters
int windowSize = 10000;
unsigned long windowStartTime;
double heInput, heOutput, heSetpoint;
double heKp = 0.5, heKi = 1, heKd = 1;

PID heater(&heInput, &heOutput, &heSetpoint, heKp, heKi, heKd, DIRECT); 

#define ONE_WIRE_BUS 2
OneWire ourWire(ONE_WIRE_BUS);
DallasTemperature sensors(&ourWire);

void printscr() {
  //Serial.println("printscreen");
    time_t t = now()- runTimer;
    //Print run time information every seconds whilst calculating PID
    lcd.setCursor(0, 0);
    lcd.print(F("Temp:"));
    lcd.print(doubleMap(averageTemp, 0, 1023, -45.2142, 80),2);
    lcd.setCursor(0, 1);
    lcd.print(F("Target: "));
    lcd.print(doubleMap(heSetpoint, 0, 1023, -45.2142, 80),2);
    Alarm.timerOnce(1, printscr);  //display every second when a program is running rather than make everything else pause
}
  
double doubleMap(double in, double A, double B, double C, double D) {
  double out;
  out = (in-A)/(B-A)*(D-C)+C;
  return out;
}  

void getTemp(){
 //returns the temperature from one DS18S20 in DEG Celsius
 sensors.requestTemperatures();
 tempTemp = doubleMap(sensors.getTempCByIndex(0), -45.2142, 80, 0, 1023);
 //hePrint = sensors.getTempCByIndex(0);
 //Serial.println(tempTemp);
 //return tempTemp;
 Alarm.timerOnce(5, getTemp);
}

void process() {
//Serial.println("process");
  totalTemp= totalTemp - readingsTemp[index];          
  readingsTemp[index] = tempTemp;
  totalTemp= totalTemp + readingsTemp[index]; 
  index++;                    
  if (index >= numReadings)              
    index = 0;                           
  averageTemp = totalTemp / numReadings; 
 Serial.println(averageTemp);
  heInput = averageTemp; 
 
  if(heInput >= heSetpoint + TBand){  //if outside of close control, use bang bang control
      heOutput = 0;
  }
  else if(heInput <= heSetpoint - TBand){
      heOutput = 1023;
  }
  else{
    heater.Compute();
  }
  Serial.println(heOutput);

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
  
}

void setup() {
  Serial.begin(9600);
  //Serial.println("setup");
  sensors.begin();
  lcd.begin(20, 4);

  //Timing                                                                                       
   setTime(0,0,0,1,1,16); // set time to Saturday 0:00:00am Jan 1 2013   // setTime to be synced to RTC
  //modes
  pinMode(14, OUTPUT);    //Heater output is digital, not PWM
    //rotary encoder
  pinMode(buttonPin, INPUT);
  pinMode(backButton, INPUT);
 // turn on pullup resistors
  //digitalWrite(buttonPin, HIGH);
  //digitalWrite(backButton, HIGH);
  //PID
  windowStartTime = millis();
  heater.SetOutputLimits(0, windowSize);
  heater.SetMode(AUTOMATIC); //Should be AUTOMATIC, only manual during testing

  //output RH and Temp values to PID and initilise
  lcd.clear();

  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readingsTemp[thisReading] = 0;  
  heSetpoint = 900.45;
  heater.SetSampleTime(5000);  //PID time between calculations in ms
  Alarm.timerOnce(1, printscr);
  Alarm.timerOnce(5, getTemp);
}

void loop() {
  //Serial.println("loop");
  process();
  Alarm.delay(10); // check if timer has expired
  if (digitalRead(buttonPin) == LOW){
    //while (digitalRead(buttonPin) == LOW) delay(10);
      heSetpoint = heSetpoint + 5;
      printscr();
  }
  else if (digitalRead(backButton) == LOW){
    //while (digitalRead(backButton) == LOW) delay(10);
    heSetpoint = heSetpoint - 5;
    printscr();
  }  
}


