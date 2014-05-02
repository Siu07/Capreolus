//28/01/14
//Version 0.0.5
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

void menuChangeEvent(MenuChangeEvent changed) //Notification of menu change
{
  Serial.print("Menu change from ");
  Serial.print(changed.from.getName());
  Serial.print(" to ");
  Serial.println(changed.to.getName());
}

void menuUseEvent(MenuUseEvent used)    //This is where you define a behaviour for a menu item
{
  Serial.print("Menu use ");
  Serial.println(used.item.getName());
  lcd.clear();
  lcd.setCursor(0, 0);
  if (used.item == selectPrg){
    //Possibly do Program Select Directly from here
    menu.moveRight(); 
  }
  else{
    if (used.item == prgMenu){
      programme++;
      if (programme>5) programme=1;    //Change to varible based on number of programs created
      Serial.print("Select Program: ");
      Serial.print(programme);
      switch (programme) {
      case 1: // Programme One - Snack Salami
          lcd.print("Snack Salami");
          Serial.println(" Snack Salami");
          break;
      case 2: // Programme Two - Saucison Sec
          lcd.print("Saucisson Sec");
          Serial.println(" Saucisson Sec");
          break;
      case 3: // Programme three - Bacon
          lcd.print("Bacon");
          Serial.println(" Bacon");
          break;
      case 4: // Programme Four - Large Meat
          lcd.print("Large Meat");
          Serial.println(" Large Meat");
          break;
      case 5: // Programme Five - Calibration
          lcd.print("Calibration");
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
            Serial.println("Plug in PC to edit names");//do something
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
                        Serial.println("Heater Selected");  //optimise with repeated varibles
                        menu.moveRight();
                      }
                      else{
                        if (used.item == cool) {
                          menuFlag = 20;
                          Serial.println("Cooler Selected");
                          lcd.print("Cooler");
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
