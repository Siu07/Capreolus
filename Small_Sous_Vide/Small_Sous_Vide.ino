#include <LiquidCrystal.h>       //LCD 20x4
#include <PID_v1.h>              //Where the magic happens
#include <math.h>                //mapping numbers between differing scales
#include <DallasTemperature.h>   //Small temperature sensing module
#include <OneWire.h>
#include <Time.h>                //Counting from boot time
#include <TimeAlarms.h>          //Basic multithreading
#include <Bounce2.h>             //Deboounce inputs

enum PinAssignments {
  buttonPin = 2,    // Select
  backButton = 3    // back
};

#define buttonPin 2
#define backButton 3

Bounce debouncer1 = Bounce(); 
Bounce debouncer2 = Bounce(); 
//Display
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

//PID
int Temp;
double TBand = 384.6;  //(+-2c) Temperature's +- this value from the target will switch control over to bang bang control
double tempTemp = 20;
int hours = 0, mins =0, secs = 0;
//int buttonState = 0;
long t = 0;

//Arduino hardware IO
int tempPin = A0;    //Define input pins
int hePin = 14;     //Define output pins

long runTimer = 0;    //milli's since program start

//heater                                      Define default PID parameters
int windowSize = 10000;
unsigned long windowStartTime;
double heInput, heOutput, heSetpoint;
double heKp = 0.5, heKi = 2, heKd = 2;

PID heater(&heInput, &heOutput, &heSetpoint, heKp, heKi, heKd, DIRECT); 

#define ONE_WIRE_BUS 8
OneWire ourWire(ONE_WIRE_BUS);
DallasTemperature sensors(&ourWire);

void printscr() {
    time_t t = now()- runTimer;
    //Print run time information every seconds whilst calculating PID
    lcd.setCursor(0, 0);
    lcd.print(F("Temp:"));
    lcd.print(doubleMap(heInput, 0, 1023, -45.2142, 80),2);
    lcd.setCursor(0, 1);
    lcd.print(F("Target: "));
    lcd.print(doubleMap(heSetpoint, 0, 1023, -45.2142, 80),2);
    lcd.setCursor(0, 2);
    lcd.print(F("Window: "));
    lcd.print((heOutput/windowSize)*100);
    lcd.print(F("%"));
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
 heInput = doubleMap(sensors.getTempCByIndex(0), -45.2142, 80, 0, 1023);
 process();
 Alarm.timerOnce(1, getTemp);
}

void process() {
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
}

void setup() {
  Serial.begin(9600);
  sensors.begin();
  lcd.begin(20, 4);
  //Timing                                                                                       
  setTime(0,0,0,1,1,16); // set time to Saturday 0:00:00am Jan 1 2013   // setTime to be synced to RTC
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(backButton, INPUT_PULLUP);
  pinMode(14, OUTPUT);    //Heater output is digital, not PWM
  digitalWrite(buttonPin, HIGH);
  digitalWrite(backButton, HIGH);
   // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(0, doButtonPin, CHANGE);
  // encoder pin on interrupt 1 (pin 3)
  attachInterrupt(1, doBackButton, CHANGE);
  debouncer1.attach(buttonPin);
  debouncer1.interval(15); // interval in ms
  debouncer2.attach(backButton);
  debouncer2.interval(15); // interval in ms

  //PID
  windowStartTime = millis();
  heater.SetOutputLimits(0, windowSize);
  heater.SetMode(AUTOMATIC); //Should be AUTOMATIC, only manual during testing
  //output RH and Temp values to PID and initilise
  lcd.clear();
  heSetpoint = 859.6;
  heater.SetSampleTime(1000);  //PID time between calculations in ms
  Alarm.timerOnce(1, getTemp);
  delay(10);
  Alarm.timerOnce(1, printscr);
}

void doButtonPin(){
  debouncer1.update();
  if (debouncer1.read() == HIGH){
    debouncer1.update();
    heSetpoint = heSetpoint + 2.0425;
    lcd.setCursor(0, 1);
    lcd.print(F("Target: "));
    lcd.print(doubleMap(heSetpoint, 0, 1023, -45.2142, 80),2);
  } 
}

void doBackButton(){
  debouncer2.update();
  if (debouncer2.read() == HIGH){
    debouncer2.update();
    heSetpoint = heSetpoint - 2.0425;
    lcd.setCursor(0, 1);
    lcd.print(F("Target: "));
    lcd.print(doubleMap(heSetpoint, 0, 1023, -45.2142, 80),2);
  }
}

void loop() {
  Alarm.delay(10); // check if timer has expired
//  if (digitalRead(buttonPin) == LOW){
//    
//  }
//  else if (digitalRead(backButton) == LOW){
//    
//  }
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
  debouncer1.update();
  debouncer2.update();
}


