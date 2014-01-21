//21/01/13
//Version 0.0.0
#include <MenuBackend.h>

MenuBackend menu = MenuBackend(menuUseEvent,menuChangeEvent);
  MenuItem selectPrg = MenuItem("Select Program");
    MenuItem prg = MenuItem("Program #");
      MenuItem runPrg = MenuItem("Program Run");
      MenuItem editPrg = MenuItem("Edit Program");
        MenuItem prgName = MenuItem("Program Name");
          MenuItem prgStep = MenuItem("Program Steps");
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
      prg.addRight(runPrg);
        runPrg.addBefore(delPrg);
        runPrg.addAfter(editPrg);
        editPrg.addAfter(delPrg);
        editPrg.addRight(prgName);
          prgName.addRight(prgStep);
            prgStep.addRight(prgSet);
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
    Serial.print("Select a Program 1-5");
  }
  else{
  if (used.item == prg){
    //do something
  }
  else{
    if (used.item == runPrg){
      //do something
    }
    else{
      if (used.item == editPrg){
        //do something
      }
      else{
        if (used.item == prgName){
          //do something
        }
        else{
          if (used.item == prgStep){
            //do something
          }
          else{
            if (used.item == prgSet){
              //do something
            }
            else{
              if (used.item == delPrg){
                //do something
              }
              else{
                if (used.item == delPrgConf){
                  //do something
                }
                else{
                  if (used.item == settings){
                    //do something
                  }
                  else{
                    if (used.item == heat){
                      //do something
                    }
                    else{
                    if (used.item == P){
                      //do something
                    }
                    else{
                    if (used.item == I){
                      //do something
                    }
                    else{
                    if (used.item == D){
                      //do something
                    }
                    else{
                    if (used.item == cool){
                      //do something
                    }
                    else{
                    if (used.item == humid){
                      //do something
                    }
                    else{
                    if (used.item == dehumid){
                      //do something
                    }
                    else{
                    if (used.item == stat){
                      //do something
                    }
}

void menuChangeEvent(MenuChangeEvent changed) //Notification of menu change
{
  Serial.print("Menu change ");
  Serial.print(changed.from.getName());
  Serial.print(" ");
  Serial.println(changed.to.getName());
}

void setup()
{
  Serial.begin(9600);

  menuSetup();
  Serial.println("Starting navigation:\r\nUp: w   Down: s   Left: a   Right: d   Use: e");
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

