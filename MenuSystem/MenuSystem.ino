//24/01/13
//Version 0.0.3
#include <MenuBackend.h>
#include <LiquidCrystal.h>
#include <PID_v1.h>
#include <PID_AutoTune_v0.h>
#include <EEPROMex.h>

//Display
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

int programme =0, menuFlag = 0, numInput = 0;
const int memBase          = 350;

//heater                                      Define Default PID parameters            //need to set all to zero and read values from EEPROM
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

PID heater(&heInput, &heOutput, &heSetpoint, heKp, heKi, heKd, DIRECT);    //initilize PIDs
PID cooler(&coInput, &coOutput, &coSetpoint, coKp, coKi, coKd, REVERSE);
PID humidifier(&huInput, &huOutput, &huSetpoint, huKp, huKi, huKd, DIRECT);
PID dehumidifier(&deInput, &deOutput, &deSetpoint, deKp, deKi, deKd, REVERSE);

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
  Serial.begin(9600);
  lcd.begin(20, 4);
  lcd.setCursor(0, 0);
  menuSetup();
  Serial.println("Starting navigation:\r\nUp: w   Down: s   Left: a   Right: d   Use: e");
  menu.moveDown();
  
  EEPROM.setMemPool(memBase, EEPROMSizeUno);
  
  heater.SetOutputLimits(0, windowSize);
  heater.SetMode(AUTOMATIC); //Should be AUTOMATIC, only manual during testing
  cooler.SetMode(AUTOMATIC);
  humidifier.SetMode(AUTOMATIC);
  dehumidifier.SetMode(AUTOMATIC);
  
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
  deKd = EEPROM.readDouble(44);
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
