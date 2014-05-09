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
    lcd.clear();
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
    lcd.clear();
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
    lcd.clear();
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
    lcd.clear();
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
    lcd.clear();
    printscr();
      }
  else if (used.item == prgSettings) {
    //
  }
  else if (used.item == PIDSettings) {
    //
  }
  else if (used.item == SetssnackSalami) {  //good code, wrong place
//    if (prgStep > 7) prgStep = 0;
//    lcd.clear();
//    lcd.setCursor(0, 0);
//    lcd.print("Snack Salami");
//    lcd.setCursor(0, 1);
//    lcd.print("Step: ");
//    lcd.print(prgStep + 1);
//    lcd.setCursor(0, 2);
//    lcd.print(" Hours   RH   Temp");
//    lcd.setCursor(1, 3);
//    lcd.print(prgOne[prgStep]);
//    lcd.setCursor(9, 3);
//    lcd.print(prgOneRH[prgStep]);
//    lcd.setCursor(14, 3);
//    lcd.print(prgOneTemp[prgStep]);
//    prgStep++;
  }
  else if (used.item == SetssaucionSec) {
//    if (prgStep > 7) prgStep = 0;
//    lcd.clear();
//    lcd.setCursor(0, 0);
//    lcd.print("Saucion Sec");
//    lcd.setCursor(0, 1);
//    lcd.print("Step: ");
//    lcd.print(prgStep + 1);
//    lcd.setCursor(0, 2);
//    lcd.print(" Hours   RH   Temp");
//    lcd.setCursor(1, 3);
//    lcd.print(prgTwo[prgStep]);
//    lcd.setCursor(9, 3);
//    lcd.print(prgTwoRH[prgStep]);
//    lcd.setCursor(14, 3);
//    lcd.print(prgTwoTemp[prgStep]);
//    prgStep++;
  }
  else if (used.item == Setsbacon) {
//    if (prgStep > 7) prgStep = 0;
//    lcd.clear();
//    lcd.setCursor(0, 0);
//    lcd.print("Bacon");
//    lcd.setCursor(0, 1);
//    lcd.print("Step: ");
//    lcd.print(prgStep + 1);
//    lcd.setCursor(0, 2);
//    lcd.print(" Hours   RH   Temp");
//    lcd.setCursor(1, 3);
//    lcd.print(prgThree[prgStep]);
//    lcd.setCursor(9, 3);
//    lcd.print(prgThreeRH[prgStep]);
//    lcd.setCursor(14, 3);
//    lcd.print(prgThreeTemp[prgStep]);
//    prgStep++;
  }
  else if (used.item == SetslargeMeat) {
//    if (prgStep > 7) prgStep = 0;
//    lcd.clear();
//    lcd.setCursor(0, 0);
//    lcd.print("Large Meat");
//    lcd.setCursor(0, 1);
//    lcd.print("Step: ");
//    lcd.print(prgStep + 1);
//    lcd.setCursor(0, 2);
//    lcd.print(" Hours   RH   Temp");
//    lcd.setCursor(1, 3);
//    lcd.print(prgFour[prgStep]);
//    lcd.setCursor(9, 3);
//    lcd.print(prgFourRH[prgStep]);
//    lcd.setCursor(14, 3);
//    lcd.print(prgFourTemp[prgStep]);
//    prgStep++;
  }
  else if (used.item == SetsHolding) {
//    if (prgStep > 7) prgStep = 0;
//    lcd.clear();
//    lcd.setCursor(0, 0);
//    lcd.print("Holding");
//    lcd.setCursor(0, 1);
//    lcd.print("Step: ");
//    lcd.print(prgStep + 1);
//    lcd.setCursor(0, 2);
//    lcd.print(" Hours   RH   Temp");
//    lcd.setCursor(1, 3);
//    lcd.print(prgFive[prgStep]);
//    lcd.setCursor(9, 3);
//    lcd.print(prgFiveRH[prgStep]);
//    lcd.setCursor(14, 3);
//    lcd.print(prgFiveTemp[prgStep]);
//    prgStep++;
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
    else if (used.item == ) {
      
      
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
    //prgStep = 0;
  }
  else if (changed.to.getName() == SetssaucionSec) {
    //prgStep = 0;
  }
  else if (changed.to.getName() == Setsbacon) {
    //prgStep = 0;
  }
  else if (changed.to.getName() == SetslargeMeat) {
    //prgStep = 0;
  }
  else if (changed.to.getName() == SetsHolding) {
    //prgStep = 0;
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

void menuSetup()    //not enough steps yet, only went up to 5 instead of 8, sigh. Also, cant have numbers before name. e.g. change "1hoursets" to "hoursets1"
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
        SetssnackSalami.addRight(hourSets1);
          hourSets1.addRight(hourSets11);
          hourSets11.addAfter(hourSets12);
          hourSets12.addAfter(hourSets13);
          hourSets13.addAfter(hourSets14);
          hourSets14.addAfter(hourSets15);
          hourSets15.addAfter(hourSets16);
          hourSets16.addAfter(hourSets17);
          hourSets17.addAfter(hourSets18);
          hourSets18.addAfter(hourSets11);
          hourSets12.addLeft(hourSets1);
          hourSets13.addLeft(hourSets1);
          hourSets14.addLeft(hourSets1);
          hourSets15.addLeft(hourSets1);
          hourSets16.addLeft(hourSets1);
          hourSets17.addLeft(hourSets1);
          hourSets18.addLeft(hourSets1);
        hourSets1.addAfter(RHSets1);
          RHSets1.addRight(RHSets11);
          RHSets11.addAfter(RHSets12);
          RHSets12.addAfter(RHSets13);
          RHSets13.addAfter(RHSets14);
          RHSets14.addAfter(RHSets15);
          RHSets15.addAfter(RHSets16);
          RHSets16.addAfter(RHSets17);
          RHSets17.addAfter(RHSets18);
          RHSets18.addAfter(RHSets11);
          RHSets12.addLeft(RHSets1);
          RHSets13.addLeft(RHSets1);
          RHSets14.addLeft(RHSets1);
          RHSets15.addLeft(RHSets1);
          RHSets16.addLeft(RHSets1);
          RHSets17.addLeft(RHSets1);
          RHSets18.addLeft(RHSets1);
        RHSets1.addAfter(TempSets1);
          TempSets1.addRight(TempSets11);
          TempSets11.addAfter(TempSets12);
          TempSets12.addAfter(TempSets13);
          TempSets13.addAfter(TempSets14);
          TempSets14.addAfter(TempSets15);
          TempSets15.addAfter(TempSets16);
          TempSets16.addAfter(TempSets17);
          TempSets17.addAfter(TempSets18);
          TempSets18.addAfter(TempSets11);
          TempSets12.addLeft(TempSets1);
          TempSets13.addLeft(TempSets1);
          TempSets14.addLeft(TempSets1);
          TempSets15.addLeft(TempSets1);
          TempSets16.addLeft(TempSets1);
          TempSets17.addLeft(TempSets1);
          TempSets18.addLeft(TempSets1);
        TempSets1.addAfter(hourSets1);
        RHSets1.addLeft(SetssnackSalami);
        TempSets1.addLeft(SetssnackSalami);
        //Call Hour/RH/Temp Settings
      SetssaucionSec.addAfter(Setsbacon);
        SetssaucionSec.addRight(hourSets2);
          hourSets2.addRight(hourSets21);
          hourSets21.addAfter(hourSets22);
          hourSets22.addAfter(hourSets23);
          hourSets23.addAfter(hourSets24);
          hourSets24.addAfter(hourSets25);
          hourSets25.addAfter(hourSets26);
          hourSets26.addAfter(hourSets27);
          hourSets27.addAfter(hourSets28);
          hourSets28.addAfter(hourSets21);
          hourSets22.addLeft(hourSets2);
          hourSets23.addLeft(hourSets2);
          hourSets24.addLeft(hourSets2);
          hourSets25.addLeft(hourSets2);
          hourSets26.addLeft(hourSets2);
          hourSets27.addLeft(hourSets2);
          hourSets28.addLeft(hourSets2);
        hourSets2.addAfter(RHSets2);
          RHSets2.addRight(RHSets21);
          RHSets21.addAfter(RHSets22);
          RHSets22.addAfter(RHSets23);
          RHSets23.addAfter(RHSets24);
          RHSets24.addAfter(RHSets25);
          RHSets25.addAfter(RHSets26);
          RHSets26.addAfter(RHSets27);
          RHSets27.addAfter(RHSets28);
          RHSets28.addAfter(RHSets21);
          RHSets22.addLeft(RHSets2);
          RHSets23.addLeft(RHSets2);
          RHSets24.addLeft(RHSets2);
          RHSets25.addLeft(RHSets2);
          RHSets26.addLeft(RHSets2);
          RHSets27.addLeft(RHSets2);
          RHSets28.addLeft(RHSets2);
        RHSets2.addAfter(TempSets2);
          TempSets2.addRight(TempSets21);
          TempSets21.addAfter(TempSets22);
          TempSets22.addAfter(TempSets23);
          TempSets23.addAfter(TempSets24);
          TempSets24.addAfter(TempSets25);
          TempSets25.addAfter(TempSets26);
          TempSets26.addAfter(TempSets27);
          TempSets27.addAfter(TempSets28);
          TempSets28.addAfter(TempSets21);
          TempSets22.addLeft(TempSets2);
          TempSets23.addLeft(TempSets2);
          TempSets24.addLeft(TempSets2);
          TempSets25.addLeft(TempSets2);
          TempSets26.addLeft(TempSets2);
          TempSets27.addLeft(TempSets2);
          TempSets28.addLeft(TempSets2);
        TempSets2.addAfter(hourSets2);
        RHSets2.addLeft(SetssaucionSec);
        TempSets2.addLeft(SetssaucionSec);
        //Call Hour/RH/Temp Settings
      Setsbacon.addAfter(SetslargeMeat);
        Setsbacon.addRight(hourSets3);
          hourSets3.addRight(hourSets31);
          hourSets31.addAfter(hourSets32);
          hourSets32.addAfter(hourSets33);
          hourSets33.addAfter(hourSets34);
          hourSets34.addAfter(hourSets35);
          hourSets35.addAfter(hourSets36);
          hourSets36.addAfter(hourSets37);
          hourSets37.addAfter(hourSets38);
          hourSets38.addAfter(hourSets31);
          hourSets32.addLeft(hourSets3);
          hourSets33.addLeft(hourSets3);
          hourSets34.addLeft(hourSets3);
          hourSets35.addLeft(hourSets3);
          hourSets36.addLeft(hourSets3);
          hourSets37.addLeft(hourSets3);
          hourSets38.addLeft(hourSets3);
        hourSets3.addAfter(RHSets3);
          RHSets3.addRight(RHSets31);
          RHSets31.addAfter(RHSets32);
          RHSets32.addAfter(RHSets33);
          RHSets33.addAfter(RHSets34);
          RHSets34.addAfter(RHSets35);
          RHSets35.addAfter(RHSets36);
          RHSets36.addAfter(RHSets37);
          RHSets37.addAfter(RHSets38);
          RHSets38.addAfter(RHSets31);
          RHSets32.addLeft(RHSets3);
          RHSets33.addLeft(RHSets3);
          RHSets34.addLeft(RHSets3);
          RHSets35.addLeft(RHSets3);
          RHSets36.addLeft(RHSets3);
          RHSets37.addLeft(RHSets3);
          RHSets38.addLeft(RHSets3);
        RHSets3.addAfter(TempSets3);
          TempSets3.addRight(TempSets31);
          TempSets31.addAfter(TempSets32);
          TempSets32.addAfter(TempSets33);
          TempSets33.addAfter(TempSets34);
          TempSets34.addAfter(TempSets35);
          TempSets35.addAfter(TempSets36);
          TempSets36.addAfter(TempSets37);
          TempSets37.addAfter(TempSets38);
          TempSets38.addAfter(TempSets31);
          TempSets32.addLeft(TempSets3);
          TempSets33.addLeft(TempSets3);
          TempSets34.addLeft(TempSets3);
          TempSets35.addLeft(TempSets3);
          TempSets36.addLeft(TempSets3);
          TempSets37.addLeft(TempSets3);
          TempSets38.addLeft(TempSets3);
        TempSets3.addAfter(hourSets3);
        RHSets3.addLeft(Setsbacon);
        TempSets3.addLeft(Setsbacon);
        //Call Hour/RH/Temp Settings
      SetslargeMeat.addAfter(SetsHolding);
        SetslargeMeat.addRight(hourSets4);
          hourSets4.addRight(hourSets41);
          hourSets41.addAfter(hourSets42);
          hourSets42.addAfter(hourSets43);
          hourSets43.addAfter(hourSets44);
          hourSets44.addAfter(hourSets45);
          hourSets45.addAfter(hourSets46);
          hourSets46.addAfter(hourSets47);
          hourSets47.addAfter(hourSets48);
          hourSets48.addAfter(hourSets41);
          hourSets42.addLeft(hourSets4);
          hourSets43.addLeft(hourSets4);
          hourSets44.addLeft(hourSets4);
          hourSets45.addLeft(hourSets4);
          hourSets46.addLeft(hourSets4);
          hourSets47.addLeft(hourSets4);
          hourSets48.addLeft(hourSets4);
        hourSets4.addAfter(RHSets4);
          RHSets4.addRight(RHSets41);
          RHSets41.addAfter(RHSets42);
          RHSets42.addAfter(RHSets43);
          RHSets43.addAfter(RHSets44);
          RHSets44.addAfter(RHSets45);
          RHSets45.addAfter(RHSets46);
          RHSets46.addAfter(RHSets47);
          RHSets47.addAfter(RHSets48);
          RHSets48.addAfter(RHSets41);
          RHSets42.addLeft(RHSets4);
          RHSets43.addLeft(RHSets4);
          RHSets44.addLeft(RHSets4);
          RHSets45.addLeft(RHSets4);
          RHSets46.addLeft(RHSets4);
          RHSets47.addLeft(RHSets4);
          RHSets48.addLeft(RHSets4);
        RHSets4.addAfter(TempSets4);
          TempSets4.addRight(TempSets41);
          TempSets41.addAfter(TempSets42);
          TempSets42.addAfter(TempSets43);
          TempSets43.addAfter(TempSets44);
          TempSets44.addAfter(TempSets45);
          TempSets45.addAfter(TempSets46);
          TempSets46.addAfter(TempSets47);
          TempSets47.addAfter(TempSets48);
          TempSets48.addAfter(TempSets41);
          TempSets42.addLeft(TempSets4);
          TempSets43.addLeft(TempSets4);
          TempSets44.addLeft(TempSets4);
          TempSets45.addLeft(TempSets4);
          TempSets46.addLeft(TempSets4);
          TempSets47.addLeft(TempSets4);
          TempSets48.addLeft(TempSets4);
        TempSets4.addAfter(hourSets4);
        RHSets4.addLeft(SetslargeMeat);
        TempSets4.addLeft(SetslargeMeat);
        //Call Hour/RH/Temp Settings
      SetsHolding.addAfter(SetssnackSalami);
        SetsHolding.addRight(hourSets5);
          hourSets5.addRight(hourSets51);
          hourSets51.addAfter(hourSets52);
          hourSets52.addAfter(hourSets53);
          hourSets53.addAfter(hourSets54);
          hourSets54.addAfter(hourSets55);
          hourSets55.addAfter(hourSets56);
          hourSets56.addAfter(hourSets57);
          hourSets57.addAfter(hourSets58);
          hourSets58.addAfter(hourSets51);
          hourSets52.addLeft(hourSets5);
          hourSets53.addLeft(hourSets5);
          hourSets54.addLeft(hourSets5);
          hourSets55.addLeft(hourSets5);
          hourSets56.addLeft(hourSets5);
          hourSets57.addLeft(hourSets5);
          hourSets58.addLeft(hourSets5);
        hourSets5.addAfter(RHSets5);
          RHSets5.addRight(RHSets51);
          RHSets51.addAfter(RHSets52);
          RHSets52.addAfter(RHSets53);
          RHSets53.addAfter(RHSets54);
          RHSets54.addAfter(RHSets55);
          RHSets55.addAfter(RHSets56);
          RHSets56.addAfter(RHSets57);
          RHSets57.addAfter(RHSets58);
          RHSets58.addAfter(RHSets51);
          RHSets52.addLeft(RHSets5);
          RHSets53.addLeft(RHSets5);
          RHSets54.addLeft(RHSets5);
          RHSets55.addLeft(RHSets5);
          RHSets56.addLeft(RHSets5);
          RHSets57.addLeft(RHSets5);
          RHSets58.addLeft(RHSets5);
        RHSets5.addAfter(TempSets5);
          TempSets5.addRight(TempSets51);
          TempSets51.addAfter(TempSets52);
          TempSets52.addAfter(TempSets53);
          TempSets53.addAfter(TempSets54);
          TempSets54.addAfter(TempSets55);
          TempSets55.addAfter(TempSets56);
          TempSets56.addAfter(TempSets57);
          TempSets57.addAfter(TempSets58);
          TempSets58.addAfter(TempSets51);
          TempSets52.addLeft(TempSets5);
          TempSets53.addLeft(TempSets5);
          TempSets54.addLeft(TempSets5);
          TempSets55.addLeft(TempSets5);
          TempSets56.addLeft(TempSets5);
          TempSets57.addLeft(TempSets5);
          TempSets58.addLeft(TempSets5);
        TempSets5.addAfter(hourSets5);
        RHSets5.addLeft(SetsHolding);
        TempSets5.addLeft(SetsHolding);
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
