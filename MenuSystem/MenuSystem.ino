//22/01/13
//Version 0.0.1
#include <MenuBackend.h>
#include <LiquidCrystal.h>

//Display
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

int programme =0, menuFlag =0;


MenuBackend menu = MenuBackend(menuUseEvent,menuChangeEvent);
  MenuItem selectPrg = MenuItem("Select Program");
    MenuItem prgMenu = MenuItem("Program #");
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
    selectPrg.addRight(prg);
      prgMenu.addRight(runPrg);
      prgMenu.addRight(editPrg);
      prgMenu.addRight(delPrg);
        runPrg.addBefore(delPrg);
        runPrg.addAfter(editPrg);
        editPrg.addAfter(delPrg);
          editPrg.addRight(prgName);
            prgName.addRight(prgStep);
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
            heat.addAfter(cool);
            cool.addRight(P);
              P.addBefore(D);
              P.addAfter(I);
              I.addAfter(D);
            cool.addAfter(humid);
            humid.addRight(P);
              P.addBefore(D);
              P.addAfter(I);
              I.addAfter(D);
            humid.addAfter(dehumid);
            dehumid.addRight(P);
              P.addBefore(D);
              P.addAfter(I);
              I.addAfter(D);
    settings.addAfter(stat);
    stat.addAfter(selectPrg);
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
      case 1:
        { // Programme One - Snack Salami
          lcd.print("Snack Salami");
          Serial.print(programme);
          Serial.println(" Snack Salami");
          break;
        }
      case 2:
        { // Programme Two - Saucison Sec
          lcd.print("Saucisson Sec");
          Serial.print(programme);
          Serial.println(" Saucisson Sec");
          break;
        }
      case 3:
        { // Programme three - Bacon
          lcd.print("Bacon");
          Serial.print(programme);
          Serial.println(" Bacon");
          break;
        }
      case 4:
        { // Programme Four - Large Meat
          lcd.print("Large Meat");
          Serial.print(programme);
          Serial.println(" Large Meat");
          break;
        }
      case 5:
        { // Programme Five - Calibration
          lcd.print("Calibration");
          Serial.print(programme);
          Serial.println(" Calibration");
          break;
        }
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
                        menuFlag = 1;
                        menu.moveRight();
                      }
                      else{
                        if (used.item == P){    //set Kp value for equipment
                          if(menuFlag == 1) {
                            Serial.println("Kp Heater");
                          }
                          else{
                            if(menuFlag == 2) {
                              Serial.println("Kp Cooler");
                            }
                            else{
                              if(menuFlag == 3) {
                                Serial.println("Kp Humidifier");
                              }
                              else{
                                if(menuFlag == 4) {
                                  Serial.println("Kp Dehumidifier");
                                }
                              }
                            }
                          }
                          //SetTunings(newKp, Ki, Kd);
                        }
                        else{
                          if (used.item == I){    //set Ki value for equipment
                            if(menuFlag == 1) {
                              Serial.println("Ki Heater");
                            }
                            else{
                              if(menuFlag == 2) {
                                Serial.println("Ki Cooler");
                              }
                              else{
                                if(menuFlag == 3) {
                                    Serial.println("Ki Humidifier");
                                }
                                else{
                                  if(menuFlag == 4) {
                                    Serial.println("Ki Dehumidifier");
                                  }
                                }
                              }
                            }
                          }
                          else{
                            if (used.item == D){    //set Kd value for equipment
                              if(menuFlag == 1) {
                                Serial.println("Kd Heater");
                              }
                              else{
                                if(menuFlag == 2) {
                                  Serial.println("Kd Cooler");
                                }
                                else{
                                  if(menuFlag == 3) {
                                      Serial.println("Kd Humidifier");
                                  }
                                  else{
                                    if(menuFlag == 4) {
                                      Serial.println("Kd Dehumidifier");
                                    }
                                  }
                                }
                              }
                            }
                            else{
                              if (used.item == cool) {
                                menuFlag = 2;
                                menu.moveRight();
                              }
                              else{
                                if (used.item == humid){
                                  menuFlag = 3;
                                  menu.moveRight();
                                }
                                else{
                                  if (used.item == dehumid){
                                    menuFlag = 4;
                                    menu.moveRight();
                                  }
                                  else{
                                    if (used.item == stat){
                                      //Display current program, current temp & humidity, step and setpoints
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

void setup()
{
  Serial.begin(9600);
  lcd.begin(20, 4);
  lcd.setCursor(0, 0);
  menuSetup();
  Serial.println("Starting navigation:\r\nUp: w   Down: s   Left: a   Right: d   Use: e");
  menu.moveDown();
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

