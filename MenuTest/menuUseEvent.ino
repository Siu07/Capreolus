void menuUseEvent(MenuUseEvent used)
{
  Serial.print("Menu use ");
  Serial.println(used.item.getName());
  if (used.item == snackSalami){
    calculating = true;
    programme = 1;
    prgSec = 0;
    prgMin = 0;
    prgHour = prgOne[prgStep];
    RH = prgOneRH[prgStep];
    Temp = prgOneTemp[prgStep];
    strcpy(name, "Snack Salami");
    prg();
    printscr();
  }
  else if (used.item == saucionSec) {
    calculating = true;
    programme = 2;
    prgSec = 0;
    prgMin = 0;
    prgHour = prgTwo[prgStep];
    RH = prgTwoRH[prgStep];
    Temp = prgTwoTemp[prgStep];
    strcpy(name, "Saucion Sec");
    prg();
    printscr();
  }
  else if (used.item == bacon) {
    calculating = true;
    programme = 3;
    prgSec = 0;
    prgMin = 0;
    prgHour = prgThree[prgStep];
    RH = prgThreeRH[prgStep];
    Temp = prgThreeTemp[prgStep];
    strcpy(name, "Bacon");
    prg();
    printscr();
  }
  else if (used.item == largeMeat) {
    calculating = true;
    programme = 4;
    prgSec = 0;
    //prgMin = prgFour[prgStep];   //TESTING
    //prgHour = 0;
    prgMin = 0;                    //ORIGINAL
    prgHour = prgFour[prgStep];
    RH = prgFourRH[prgStep];
    Temp = prgFourTemp[prgStep];
    strcpy(name, "Large Meat");
    prg();
    printscr();
  }
  else if (used.item == Holding) {
    calculating = true;
    programme = 5;
    prgSec = 0;
    prgMin = 0;
    prgHour = prgFive[prgStep];
    RH = prgFiveRH[prgStep];
    Temp = prgFiveTemp[prgStep];
    strcpy(name, "Holding");
    prg();
    printscr();
      }
  else if (used.item == prgSettings) {
    //
  }
  else if (used.item == PIDSettings) {
    //
  }
  else if (used.item == SetssnackSalami) {
    if (prgStep > 7) prgStep = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Snack Salami");
    lcd.setCursor(0, 1);
    lcd.print("Step: ");
    lcd.print(prgStep + 1);
    lcd.setCursor(0, 2);
    lcd.print(" Hours   RH   Temp");
    lcd.setCursor(1, 3);
    lcd.print(prgOne[prgStep]);
    lcd.setCursor(9, 3);
    lcd.print(prgOneRH[prgStep]);
    lcd.setCursor(14, 3);
    lcd.print(prgOneTemp[prgStep]);
    prgStep++;
  }
  else if (used.item == SetssaucionSec) {
    if (prgStep > 7) prgStep = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Saucion Sec");
    lcd.setCursor(0, 1);
    lcd.print("Step: ");
    lcd.print(prgStep + 1);
    lcd.setCursor(0, 2);
    lcd.print(" Hours   RH   Temp");
    lcd.setCursor(1, 3);
    lcd.print(prgTwo[prgStep]);
    lcd.setCursor(9, 3);
    lcd.print(prgTwoRH[prgStep]);
    lcd.setCursor(14, 3);
    lcd.print(prgTwoTemp[prgStep]);
    prgStep++;
  }
  else if (used.item == Setsbacon) {
    if (prgStep > 7) prgStep = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Bacon");
    lcd.setCursor(0, 1);
    lcd.print("Step: ");
    lcd.print(prgStep + 1);
    lcd.setCursor(0, 2);
    lcd.print(" Hours   RH   Temp");
    lcd.setCursor(1, 3);
    lcd.print(prgThree[prgStep]);
    lcd.setCursor(9, 3);
    lcd.print(prgThreeRH[prgStep]);
    lcd.setCursor(14, 3);
    lcd.print(prgThreeTemp[prgStep]);
    prgStep++;
  }
  else if (used.item == SetslargeMeat) {
    if (prgStep > 7) prgStep = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Large Meat");
    lcd.setCursor(0, 1);
    lcd.print("Step: ");
    lcd.print(prgStep + 1);
    lcd.setCursor(0, 2);
    lcd.print(" Hours   RH   Temp");
    lcd.setCursor(1, 3);
    lcd.print(prgFour[prgStep]);
    lcd.setCursor(9, 3);
    lcd.print(prgFourRH[prgStep]);
    lcd.setCursor(14, 3);
    lcd.print(prgFourTemp[prgStep]);
    prgStep++;
  }
  else if (used.item == SetsHolding) {
    if (prgStep > 7) prgStep = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Holding");
    lcd.setCursor(0, 1);
    lcd.print("Step: ");
    lcd.print(prgStep + 1);
    lcd.setCursor(0, 2);
    lcd.print(" Hours   RH   Temp");
    lcd.setCursor(1, 3);
    lcd.print(prgFive[prgStep]);
    lcd.setCursor(9, 3);
    lcd.print(prgFiveRH[prgStep]);
    lcd.setCursor(14, 3);
    lcd.print(prgFiveTemp[prgStep]);
    prgStep++;
  }
  else if (used.item == versionI) {
    //userInput(11, 1);
    lcd.setCursor(0, 0);
    lcd.print("Build ");
    lcd.print(Version);
    lcd.setCursor(0, 2);
    lcd.print("By Chris Davies");
    lcd.setCursor(0, 3);
    lcd.print("siu07crd@gmail.com");
  }
  else if (used.item == prgHe) {
    menuFlag = 10;
  }
  else if (used.item == prgCo) {
    menuFlag = 20;
  }
  else if (used.item == prgHu) {
    menuFlag = 30;
  }
  else if (used.item == prgDe) {
    menuFlag = 40;
  }
  else if (used.item == Phe) {
    userInput(11,0.1);
    lcd.clear();
    menu.moveLeft();
  }
  else if (used.item == Ihe) {
    userInput(12,0.1);
    lcd.clear();
    menu.moveLeft();
  }
  else if (used.item == Dhe) {
    userInput(13,0.1);
    lcd.clear();
    menu.moveLeft();
  }
  else if (used.item == Pco) {
    userInput(21,0.1);
    lcd.clear();
    menu.moveLeft();
  }
  else if (used.item == Ico) {
    userInput(22,0.1);
    lcd.clear();
    menu.moveLeft();
  }
  else if (used.item == Dco) {
    userInput(23,0.1);
    lcd.clear();
    menu.moveLeft();
  }
  else if (used.item == Phu) {
    userInput(31,0.1);
    lcd.clear();
    menu.moveLeft();
  }
  else if (used.item == Ihu) {
    userInput(32,0.1);
    lcd.clear();
    menu.moveLeft();
  }
  else if (used.item == Dhu) {
    userInput(33,0.1);
    lcd.clear();
    menu.moveLeft();
  }
  else if (used.item == Pde) {
    userInput(41,0.1);
    lcd.clear();
    menu.moveLeft();
  }
  else if (used.item == Ide) {
    userInput(42,0.1);
    lcd.clear();
    menu.moveLeft();
  }
  else if (used.item == Dde) {
    userInput(43,0.1);
    lcd.clear();
    menu.moveLeft();
  }
}

void menuChangeEvent(MenuChangeEvent changed)
{
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print(changed.to.getName());
  Serial.println(changed.to.getName());
  if (changed.to.getName() == prgHe) {
    lcd.setCursor(0, 0);
    lcd.print("PID settings");
    lcd.setCursor(0, 2);
    lcd.print("   P     I     D");
    lcd.setCursor(2, 3);
    lcd.print(heKp);
    lcd.setCursor(8, 3);
    lcd.print(heKi);
    lcd.setCursor(14, 3);
    lcd.print(heKd);
  }
  else if (changed.to.getName() == prgCo) {
    lcd.setCursor(0, 0);
    lcd.print("PID settings");
    lcd.setCursor(0, 2);
    lcd.print("   P     I     D");
    lcd.setCursor(2, 3);
    lcd.print(coKp);
    lcd.setCursor(8, 3);
    lcd.print(coKi);
    lcd.setCursor(14, 3);
    lcd.print(coKd);
  }
  else if (changed.to.getName() == prgHu) {
    lcd.setCursor(0, 0);
    lcd.print("PID settings");
    lcd.setCursor(0, 2);
    lcd.print("   P     I     D");
    lcd.setCursor(2, 3);
    lcd.print(huKp);
    lcd.setCursor(8, 3);
    lcd.print(huKi);
    lcd.setCursor(14, 3);
    lcd.print(huKd);
  }
  else if (changed.to.getName() == prgDe) {
    lcd.setCursor(0, 0);
    lcd.print("PID settings");
    lcd.setCursor(0, 2);
    lcd.print("   P     I     D");
    lcd.setCursor(2, 3);
    lcd.print(deKp);
    lcd.setCursor(8, 3);
    lcd.print(deKi);
    lcd.setCursor(14, 3);
    lcd.print(deKd);
  }
  else if (changed.to.getName() == SetssnackSalami) {
    //
  }
  else if (changed.to.getName() == SetssaucionSec) {
    //
  }
  else if (changed.to.getName() == Setsbacon) {
    //
  }
  else if (changed.to.getName() == SetslargeMeat) {
    //
  }
  else if (changed.to.getName() == SetsHolding) {
    //
  }
  else if (changed.to.getName() == versionI) {
    lcd.setCursor(0, 0);
    lcd.print("Build ");
    lcd.print(Version);
    lcd.setCursor(0, 2);
    lcd.print("By Chris Davies");
    lcd.setCursor(0, 3);
    lcd.print("siu07crd@gmail.com");
  }
  else if (changed.to.getName() == Phe) {
    lcd.clear();
    lcd.print("Heater P: ");
    lcd.print(heKp);
  }
  else if (changed.to.getName() == Ihe) {
    lcd.clear();
    lcd.print("Heater I: ");
    lcd.print(heKi);
  }
  else if (changed.to.getName() == Dhe) {
    lcd.clear();
    lcd.print("Heater D: ");
    lcd.print(heKd);
  }
  else if (changed.to.getName() == Pco) {
    lcd.clear();
    lcd.print("Cooler P: ");
    lcd.print(coKp);
  }
  else if (changed.to.getName() == Ico) {
    lcd.clear();
    lcd.print("Cooler I: ");
    lcd.print(coKi);
  }
  else if (changed.to.getName() == Dco) {
    lcd.clear();
    lcd.print("Cooler D: ");
    lcd.print(coKd);
  }
  else if (changed.to.getName() == Phu) {
    lcd.clear();
    lcd.print("Humidifier P: ");
    lcd.print(huKp);
  }
  else if (changed.to.getName() == Ihu) {
    lcd.clear();
    lcd.print("Humidifier I: ");
    lcd.print(huKi);
  }
  else if (changed.to.getName() == Dhu) {
    lcd.clear();
    lcd.print("Humidifier D: ");
    lcd.print(huKd);
  }
  else if (changed.to.getName() == Pde) {
    lcd.clear();
    lcd.print("Dehumidifier P: ");
    lcd.print(deKp);
  }
  else if (changed.to.getName() == Ide) {
    lcd.clear();
    lcd.print("Dehumidifier I: ");
    lcd.print(deKi);
  }
  else if (changed.to.getName() == Dde) {
    lcd.clear();
    lcd.print("Dehumidifier D: ");
    lcd.print(deKd);
  }
}

void menuSetup()
{
  menu.getRoot().add(snackSalami);
    snackSalami.addAfter(saucionSec);
      //call printscreen
    saucionSec.addAfter(bacon);
      //call printscreen
    bacon.addAfter(largeMeat);
      //call printscreen
    largeMeat.addAfter(Holding);
      //call printscreen
    Holding.addAfter(prgSettings);
      //call printscreen
    prgSettings.addAfter(PIDSettings);
      prgSettings.addRight(SetssnackSalami);
      SetssnackSalami.addAfter(SetssaucionSec);
        //Call Hour/RH/Temp Settings
      SetssaucionSec.addAfter(Setsbacon);
        //Call Hour/RH/Temp Settings
      Setsbacon.addAfter(SetslargeMeat);
        //Call Hour/RH/Temp Settings
      SetslargeMeat.addAfter(SetsHolding);
        //Call Hour/RH/Temp Settings
      SetsHolding.addAfter(SetssnackSalami);
        //Call Hour/RH/Temp Settings
      SetssaucionSec.addLeft(prgSettings);
      Setsbacon.addLeft(prgSettings);
      SetslargeMeat.addLeft(prgSettings);
      SetsHolding.addLeft(prgSettings);
    PIDSettings.addAfter(versionI);
      PIDSettings.addRight(prgHe);
        prgHe.addRight(Phe);
        Phe.addAfter(Ihe);
        Ihe.addAfter(Dhe);
        Dhe.addAfter(Phe);
        Ihe.addLeft(prgHe);
        Dhe.addLeft(prgHe);
      prgHe.addAfter(prgCo);
        prgCo.addRight(Pco);
        Pco.addAfter(Ico);
        Ico.addAfter(Dco);
        Dco.addAfter(Pco);
        Ico.addLeft(prgCo);
        Dco.addLeft(prgCo);
      prgCo.addAfter(prgHu);
        prgHu.addRight(Phu);
        Phu.addAfter(Ihu);
        Ihu.addAfter(Dhu);
        Dhu.addAfter(Phu);
        Ihu.addLeft(prgHu);
        Dhu.addLeft(prgHu);
      prgHu.addAfter(prgDe);
        prgDe.addRight(Pde);
        Pde.addAfter(Ide);
        Ide.addAfter(Dde);
        Dde.addAfter(Pde);
        Ide.addLeft(prgDe);
        Dde.addLeft(prgDe);
      prgDe.addAfter(prgHe);
      prgCo.addLeft(PIDSettings);
      prgHu.addLeft(PIDSettings);
      prgDe.addLeft(PIDSettings);
    versionI.addAfter(snackSalami);
      //print version
}
