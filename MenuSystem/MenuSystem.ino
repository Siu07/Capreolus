//27/01/13
//Version 0.0.4
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
//#include <PID_AutoTune_v0.h>   Hope to integrate, but not yet.
#include <EEPROMex.h>            //Saving double varibles to EEPROM
#include <Time.h>                //Counting from boot time
#include <TimeAlarms.h>          //Basic multithreading
#include <Bounce2.h>             //Cleaning User inputs
#include <math.h>                //mapping numbers between differing scales

//Display
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
//Menu system
int programme =0, menuFlag = 0, numInput = 0;
//EEPROM
const int memBase = 350;
//PID
int RH, Temp;
int prgStep, prgHour, prgMin;
double deadband = 0.01; //set the deadband to prevent PID controllers fighting.
int hours = 0, mins =0, secs = 0;
int buttonState = 0;
const int buttonPin = 20;
long t = 0;
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
//IO - output
int hePin = 40;     //Define output pins
int coPin = 8;
int huPin = 9;
int dePin = 10;
//User Input
int lastDebounceTime = 0;  // the last time the output pin was toggled
int debounceDelay = 50;    // the debounce time; increase if the output flickers
int storedPrg = 0;
int encoderPin1 = 2;
int encoderPin2 = 3;
int oldValue = 0;
volatile int lastEncoded = 0;
volatile long encoderValue = 0;
long lastencoderValue = 0;
int sum = 0;      //encoder sum, terrible name i know
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
//Programme One - Snack Salami          Eventually, array shall not be confined to 8 steps
int prgOne[8] = {
  2, 36, 6, 6, 6, 6, 36, 144}; // Hours
int prgOneRH[8] = {
  85, 92, 88, 85, 82, 80, 80, 80}; // Relative Humidity
int prgOneTemp[8] = {
  22, 25, 22, 20, 17, 14, 12, 10}; // Temperature
// Programme Two - Saucison Sec
int prgTwo[8] = {
  2, 48, 6, 6, 6, 12, 72, 144}; // Hours
int prgTwoRH[8] = {
  85, 92, 88, 85, 82, 80, 80, 80}; // Relative Humidity
int prgTwoTemp[8] = {
  22, 25, 22, 20, 17, 14, 12, 10}; // Temperature
// Programme three - Bacon
int prgThree[8] = {
  2, 12, 3, 3, 3, 3, 3, 9}; // Hours
int prgThreeRH[8] = {
  85, 92, 88, 85, 82, 80, 80, 80}; // Relative Humidity
int prgThreeTemp[8] = {
  22, 25, 22, 20, 17, 14, 12, 10}; // Temperature
// Programme Four - Large Meat
int prgFour[8] = {
  2, 60, 12, 12, 12, 12, 48, 132}; // Hours
int prgFourRH[8] = {
  85, 92, 88, 85, 82, 80, 80, 80}; // Relative Humidity
int prgFourTemp[8] = {
  22, 25, 22, 20, 17, 14, 12, 10}; // Temperature
//Calibration Program
int prgFive[8] = {
  59, 59, 59, 59, 59, 59, 59, 59}; // Seconds
int prgFiveRH[] = {
  80, 80, 80, 80, 80, 80, 80, 80}; // Relative Humidity
int prgFiveTemp[8] = {
  20, 20, 20, 20, 20, 20, 20, 20}; // Temperature

void menuSetup()
{
  menu.getRoot().add(selectPrg);
    selectPrg.addRight(prgMenu);
      prgMenu.addRight(runPrg);
      prgMenu.addRight(editPrg);
      prgMenu.addRight(delPrg);
        runPrg.addBefore(delPrg);
        runPrg.addAfter(editPrg);
        editPrg.addAfter(delPrg);
          editPrg.addRight(prgName);
            prgName.addRight(prgMenuStep);
             prgMenuStep.addRight(prgSet);
        delPrg.addAfter(runPrg);
        delPrg.addRight(delPrgConf);
   selectPrg.addBefore(stat);
   selectPrg.addAfter(settings);
        settings.addRight(heat);
          heat.addBefore(dehumid);
            heat.addRight(P);
              P.addBefore(D);
              P.addAfter(I);
              I.addAfter(D);
              I.addLeft(heat);
              D.addLeft(heat);
            heat.addAfter(cool);
            cool.addRight(P);
              P.addBefore(D);
              P.addAfter(I);
              I.addAfter(D);
              I.addLeft(cool);
              D.addLeft(cool);
            cool.addAfter(humid);
            humid.addRight(P);
              P.addBefore(D);
              P.addAfter(I);
              I.addAfter(D);
              I.addLeft(humid);
              D.addLeft(humid);
            humid.addAfter(dehumid);
            dehumid.addRight(P);
              P.addBefore(D);
              P.addAfter(I);
              I.addAfter(D);
              I.addLeft(dehumid);
              D.addLeft(dehumid);
            cool.addLeft(settings);
            humid.addLeft(settings);
            dehumid.addLeft(settings);
    settings.addAfter(stat);
    stat.addAfter(selectPrg);
}

void setup()
{
  setTime(0,0,0,1,1,14);
  Serial.begin(9600);
  lcd.begin(20, 4);
  lcd.setCursor(0, 0);
  menuSetup();
  Serial.println("Starting navigation:\r\nUp: w   Down: s   Left: a   Right: d   Use: e");
  menu.moveDown();
  
  EEPROM.setMemPool(memBase, EEPROMSizeMega); //minimum and maximum address to use. To be used with address allocation.
                                              //Min address should be set to the last manual known entry.
  heater.SetOutputLimits(0, windowSize);
  heater.SetMode(AUTOMATIC); //Should be AUTOMATIC, only manual during testing
  cooler.SetMode(AUTOMATIC);
  humidifier.SetMode(AUTOMATIC);
  dehumidifier.SetMode(AUTOMATIC);
  windowStartTime = millis();
  //Repopulate PID values from memory. Double occupies 4 bytes. Arduino Due board would require 8.
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
  deKd = EEPROM.readDouble(44); // To be replaced by something like this: double heKp = EEPROM.getAddress(sizeof(double));
  //User I/O
  pinMode(buttonPin, INPUT);  //select button (within rotary encoder) interrupt 3
  pinMode(A0, INPUT);     //Temperature input
  pinMode(A1, INPUT);     //RH input
  pinMode(A2, INPUT);     //Calibration Input
  pinMode(40, OUTPUT);    //Heater output
  pinMode(8, OUTPUT);     //Cooler Output
  pinMode(9, OUTPUT);     //Humidifier Output
  pinMode(10, OUTPUT);    //Dehumidifier Output
  //Rotary encoder
  pinMode(encoderPin1, INPUT); 
  pinMode(encoderPin2, INPUT);
  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH); //turn pullup resistor on
  attachInterrupt(0, updateEncoder, CHANGE); 
  attachInterrupt(1, updateEncoder, CHANGE);
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
  process();
  Alarm.delay(10);  //non zero to avoid bug with libary
}

void userInput() {
  Serial.println("Enter a Value");
  while(!Serial.available()) {    //Loop traps code until input is received
    delay(100);
  }
   numInput = Serial.parseInt();
   Serial.print("you entered: ");
   Serial.println(numInput);
  //Read the user's input
  Serial.print("Previous value: ");
  
  switch (menuFlag) {  //case number, first digit: equipment type, second didgit: P,I or D
    case 11: 
      Serial.println(heKp);
      heKp = EEPROM.updateDouble(0,numInput);
      heater.SetTunings(numInput,heKi,heKd);// Heater Kp
      menuFlag = menuFlag - 1;
      Serial.print("Heater Kp changed to: ");
      break;
    case 12: 
      Serial.println(heKi);
      heKi = EEPROM.updateDouble(4,numInput);
      heater.SetTunings(heKp,numInput,heKd);// Heater Ki
      menuFlag = menuFlag - 2;
      Serial.print("Heater Ki changed to: ");
      break;
    case 13: 
      Serial.println(heKd);
      heKd = EEPROM.updateDouble(8,numInput);
      heater.SetTunings(heKp,heKi,numInput);// Heater Kd
      menuFlag = menuFlag - 3;
      Serial.print("Heater Kd changed to: ");
      break;
    case 21: 
      Serial.println(coKp);
      coKp = EEPROM.updateDouble(12,numInput);
      cooler.SetTunings(numInput,coKi,coKd);// Cooler Kp
      menuFlag = menuFlag - 1;
      Serial.print("Cooler Kp changed to: ");
      break;
    case 22: 
      Serial.println(coKi);
      coKi = EEPROM.updateDouble(16,numInput);
      cooler.SetTunings(coKp,numInput,coKd);// Cooler Ki
      menuFlag = menuFlag - 2;
      Serial.print("Cooler Ki changed to: ");
      break;
    case 23: 
      Serial.println(coKd);
      coKd = EEPROM.updateDouble(20,numInput);
      cooler.SetTunings(coKp,coKi,numInput);// Cooler Kd
      menuFlag = menuFlag - 3;
      Serial.print("Cooler Kd changed to: ");
      break;
    case 31: 
      Serial.println(huKp);
      huKp = EEPROM.updateDouble(24,numInput);
      humidifier.SetTunings(numInput,huKi,huKd);//Humidifier Kp
      menuFlag = menuFlag - 1;
      Serial.print("Humidifier Kp changed to: ");
      break;
    case 32: 
      Serial.println(huKi);
      huKi = EEPROM.updateDouble(28,numInput);
      humidifier.SetTunings(huKp,numInput,huKd);// Humidifier Ki
      menuFlag = menuFlag - 2;
      Serial.print("Humidifier Ki changed to: ");
      break;
    case 33: 
      Serial.println(huKd);
      huKd = EEPROM.updateDouble(32,numInput);
      humidifier.SetTunings(huKp,huKi,numInput);// Humidifier Kd
      menuFlag = menuFlag - 3;
      Serial.print("Humidifier Kd changed to: ");
      break;
    case 41: 
      Serial.println(deKp);
      deKp = EEPROM.updateDouble(36,numInput);
      dehumidifier.SetTunings(numInput,deKi,deKd);// Dehumidifier Kp
      menuFlag = menuFlag - 1;
      Serial.print("Dehumidifier Kp changed to: ");
      break;
    case 42: 
      Serial.println(deKi);
      deKi = EEPROM.updateDouble(40,numInput);
      dehumidifier.SetTunings(deKp,numInput,deKd);// Dehumidifier Ki
      menuFlag = menuFlag - 2;
      Serial.print("Dehumidifier Ki changed to: ");
      break;
    case 43: 
      Serial.println(deKd);
      deKd = EEPROM.updateDouble(44,numInput);//double update for double the fun.
      dehumidifier.SetTunings(deKp,deKi,numInput);// Dehumidifier Kd
      menuFlag = menuFlag - 3;
      Serial.print("Dehumidifier Kd changed to: ");
      break;
     default:
       Serial.println("Something went wrong!");
      //
  }
  Serial.println(numInput);
}

void menuUseEvent(MenuUseEvent used)    //This is where you define a behaviour for a menu item
{
  Serial.print("Menu use ");
  Serial.println(used.item.getName());
  if (used.item == selectPrg){
    //Possibly do Program Select Directly from here
    menu.moveRight(); 
  }
  else{
    if (used.item == prgMenu){
      programme++;
      if (programme>5) programme=1;    //Change to varible based on number of programs created
      Serial.print("Select Program: ");
      switch (programme) {
      case 1: // Programme One - Snack Salami
          lcd.print("Snack Salami");
          Serial.print(programme);
          Serial.println(" Snack Salami");
          break;
      case 2: // Programme Two - Saucison Sec
          lcd.print("Saucisson Sec");
          Serial.print(programme);
          Serial.println(" Saucisson Sec");
          break;
      case 3: // Programme three - Bacon
          lcd.print("Bacon");
          Serial.print(programme);
          Serial.println(" Bacon");
          break;
      case 4: // Programme Four - Large Meat
          lcd.print("Large Meat");
          Serial.print(programme);
          Serial.println(" Large Meat");
          break;
      case 5: // Programme Five - Calibration
          lcd.print("Calibration");
          Serial.print(programme);
          Serial.println(" Calibration");
          break;
      }
    }
    else{
      if (used.item == runPrg){
        //Start up PID.
        //need code to keep PID paused until this is selected
      }
      else{
        if (used.item == editPrg){
          menu.moveRight(); 
        }
        else{
          if (used.item == prgName){
            Serial.print("Plug in PC to edit names");//do something
          }
          else{
            if (used.item ==prgMenuStep){
              //Select the step to edit
            }
            else{
              if (used.item == prgSet){
                //Select the Temperature and humidity setpoints
              }
              else{
                if (used.item == delPrg){
                  //do something
                }
                else{
                  if (used.item == delPrgConf){
                    //conformation screen to prevent accidental deletion
                  }
                  else{
                    if (used.item == settings){
                      menu.moveRight(); 
                    }
                    else{
                      if (used.item == heat){
                        menuFlag = 10;
                        Serial.println("Heater Selected");
                        menu.moveRight();
                      }
                      else{
                        if (used.item == cool) {
                          menuFlag = 20;
                          Serial.println("Cooler Selected");
                          menu.moveRight();
                        }
                        else{
                          if (used.item == humid){
                            menuFlag = 30;
                            Serial.println("Humidifier Selected");
                            menu.moveRight();
                          }
                          else{
                            if (used.item == dehumid){
                              menuFlag = 40;
                              Serial.println("Dehumidifier Selected");
                              menu.moveRight();
                            }
                            else{
                              if (used.item == stat){
                                //Display current program, current temp & humidity, step and setpoints
                              }
                              else{
                              if (used.item == P){    //set Kp value for equipment
                                Serial.println("Kp Selected");
                                menuFlag = menuFlag + 1;
                                userInput();//input function
                                //SetTunings(newKp, Ki, Kd);
                                }
                                else{
                                  if (used.item == I){    //set Ki value for equipment
                                    Serial.println("Ki Selected");
                                    menuFlag = menuFlag + 2;
                                    userInput();//input function
                                    //SetTunings(Kp, newKi, Kd);
                                  }
                                  else{
                                    if (used.item == D){    //set Kd value for equipment
                                      Serial.println("Kd Selected");
                                      menuFlag = menuFlag + 3;
                                      userInput();//input function
                                      //SetTunings(Kp, Ki, newKd);
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

void menuChangeEvent(MenuChangeEvent changed) //Notification of menu change
{
  Serial.print("Menu change from ");
  Serial.print(changed.from.getName());
  Serial.print(" to ");
  Serial.println(changed.to.getName());
}

void updateEncoder(){  //detects and interperates encoder position
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

void process() {    //Calculate inputs ready for PID compute and set outputs
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

double doubleMap(double in, double A, double B, double C, double D) {  //Converts values between different scales
  double out;                                                          //doubleMap(Input,original min, original max, new min, new max);
  out = (in-A)/(B-A)*(D-C)+C;                                          //example: input 5 in range 0-100, want range 0-500.
  return out;                                                          //doubleMap(5,0,100,0,500);
}                                                                      //output = (5-0)/(100-0)*(500-0)+0 = 25
