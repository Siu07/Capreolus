void menuUseEvent(MenuUseEvent used)
{
  Serial.print(F("Menu use "));
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
//    lcd.print(F("Snack Salami"));
//    lcd.setCursor(0, 1);
//    lcd.print(F("Step: "));
//    lcd.print(prgStep + 1);
//    lcd.setCursor(0, 2);
//    lcd.print(F(" Hours   RH   Temp"));
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
//    lcd.print(F("Saucion Sec"));
//    lcd.setCursor(0, 1);
//    lcd.print(F("Step: "));
//    lcd.print(prgStep + 1);
//    lcd.setCursor(0, 2);
//    lcd.print(F(" Hours   RH   Temp"));
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
//    lcd.print(F("Bacon"));
//    lcd.setCursor(0, 1);
//    lcd.print(F("Step: "));
//    lcd.print(prgStep + 1);
//    lcd.setCursor(0, 2);
//    lcd.print(F(" Hours   RH   Temp"));
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
//    lcd.print(F("Large Meat"));
//    lcd.setCursor(0, 1);
//    lcd.print(F("Step: "));
//    lcd.print(prgStep + 1);
//    lcd.setCursor(0, 2);
//    lcd.print(F(" Hours   RH   Temp"));
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
//    lcd.print(F("Holding"));
//    lcd.setCursor(0, 1);
//    lcd.print(F("Step: "));
//    lcd.print(prgStep + 1);
//    lcd.setCursor(0, 2);
//    lcd.print(F(" Hours   RH   Temp"));
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
    lcd.print(F("Build "));
    lcd.print(Version);
    lcd.setCursor(0, 2);
    lcd.print(F("By Chris Davies"));
    lcd.setCursor(0, 3);
    lcd.print(F("siu07crd@gmail.com"));
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
  else if (used.item == hourSets11) {    //hours 1
    userInput(51,1);
    //prgOne[0]
  }
  else if (used.item == hourSets12) {
    userInput(52,1);
    //prgOne[1]
  }
  else if (used.item == hourSets13) {
    userInput(53,1);
    //prgOne[2]
  }
  else if (used.item == hourSets14) {
    userInput(54,1);
    //prgOne[3]
  }
  else if (used.item == hourSets15) {
    userInput(55,1);
    //prgOne[4]
  }
  else if (used.item == hourSets16) {
    userInput(56,1);
    //prgOne[5]
  }
  else if (used.item == hourSets17) {
    userInput(57,1);
    //prgOne[6]
  }
  else if (used.item == hourSets18) {
    userInput(58,1);
    //prgOne[7]
  }
  else if (used.item == hourSets21) {    //hours 2
    userInput(61,1);
    //prgTwo[0]
  }
  else if (used.item == hourSets22) {
    userInput(62,1);
    //prgTwo[1]
  }
  else if (used.item == hourSets23) {
    userInput(63,1);
    //prgTwo[2]
  }
  else if (used.item == hourSets24) {
    userInput(64,1);
    //prgTwo[3]
  }
  else if (used.item == hourSets25) {
    userInput(65,1);
    //prgTwo[4]
  }
  else if (used.item == hourSets26) {
    userInput(66,1);
    //prgTwo[5]
  }
  else if (used.item == hourSets27) {
    userInput(67,1);
    //prgTwo[6]
  }
  else if (used.item == hourSets28) {
    userInput(68,1);
    //prgTwo[7]
  }
  else if (used.item == hourSets31) {    //hours 3
    userInput(71,1);
    //prgThree[0]
  }
  else if (used.item == hourSets32) {
    userInput(72,1);
    //prgThree[1]
  }
  else if (used.item == hourSets33) {
    userInput(73,1);
    //prgThree[2]
  }
  else if (used.item == hourSets34) {
    userInput(74,1);
    //prgThree[3]
  }
  else if (used.item == hourSets35) {
    userInput(75,1);
    //prgThree[4]
  }
  else if (used.item == hourSets36) {
    userInput(76,1);
    //prgThree[5]
  }
  else if (used.item == hourSets37) {
    userInput(77,1);
    //prgThree[6]
  }
  else if (used.item == hourSets38) {
    userInput(78,1);
    //prgThree[7]
  }
  else if (used.item == hourSets41) {    //hours 4
    userInput(81,1);
    //prgFour[0]
  }
  else if (used.item == hourSets42) {
    userInput(82,1);
    //prgFour[1]
  }
  else if (used.item == hourSets43) {
    userInput(83,1);
    //prgFour[2]
  }
  else if (used.item == hourSets44) {
    userInput(84,1);
    //prgFour[3]
  }
  else if (used.item == hourSets45) {
    userInput(85,1);
    //prgFour[4]
  }
  else if (used.item == hourSets46) {
    userInput(86,1);
    //prgFour[5]
  }
  else if (used.item == hourSets47) {
    userInput(87,1);
    //prgFour[6]
  }
  else if (used.item == hourSets48) {
    userInput(88,1);
    //prgFour[7]
  }
  else if (used.item == hourSets51) {    //hours 5
    userInput(91,1);
    //prgFive[0]
  }
  else if (used.item == hourSets52) {
    userInput(92,1);
    //prgFive[1]
  }
  else if (used.item == hourSets53) {
    userInput(93,1);
    //prgFive[2]
  }
  else if (used.item == hourSets54) {
    userInput(94,1);
    //prgFive[3]
  }
  else if (used.item == hourSets55) {
    userInput(95,1);
    //prgFive[4]
  }
  else if (used.item == hourSets56) {
    userInput(96,1);
    //prgFive[5]
  }
  else if (used.item == hourSets57) {
    userInput(97,1);
    //prgFive[6]
  }
  else if (used.item == hourSets58) {
    userInput(98,1);
    //prgFive[7]
  }
  else if (used.item == RHSets11) {    //RH 1
    userInput(101,1);
    //prgOneRH[0]
  }
  else if (used.item == RHSets12) {
    userInput(102,1);
    //prgOneRH[1]
  }
  else if (used.item == RHSets13) {
    userInput(103,1);
    //prgOneRH[2]
  }
  else if (used.item == RHSets14) {
    userInput(104,1);
    //prgOneRH[3]
  }
  else if (used.item == RHSets15) {
    userInput(105,1);
    //prgOneRH[4]
  }
  else if (used.item == RHSets16) {
    userInput(106,1);
    //prgOneRH[5]
  }
  else if (used.item == RHSets17) {
    userInput(107,1);
    //prgOneRH[6]
  }
  else if (used.item == RHSets18) {
    userInput(108,1);
    //prgOneRH[7]
  }
  else if (used.item == RHSets21) {    //RH 2
    userInput(111,1);
    //prgTwoRH[0]
  }
  else if (used.item == RHSets22) {
    userInput(112,1);
    //prgTwoRH[1]
  }
  else if (used.item == RHSets23) {
    userInput(113,1);
    //prgTwoRH[2]
  }
  else if (used.item == RHSets24) {
    userInput(114,1);
    //prgTwoRH[3]
  }
  else if (used.item == RHSets25) {
    userInput(115,1);
    //prgTwoRH[4]
  }
  else if (used.item == RHSets26) {
    userInput(116,1);
    //prgTwoRH[5]
  }
  else if (used.item == RHSets27) {
    userInput(117,1);
    //prgTwoRH[6]
  }
  else if (used.item == RHSets28) {
    userInput(118,1);
    //prgTwoRH[7]
  }
  else if (used.item == RHSets31) {    //RH 3
    userInput(121,1);
    //prgThreeRH[0]
  }
  else if (used.item == RHSets32) {
    userInput(122,1);
    //prgThreeRH[1]
  }
  else if (used.item == RHSets33) {
    userInput(123,1);
    //prgThreeRH[2]
  }
  else if (used.item == RHSets34) {
    userInput(124,1);
    //prgThreeRH[3]
  }
  else if (used.item == RHSets35) {
    userInput(125,1);
    //prgThreeRH[4]
  }
  else if (used.item == RHSets36) {
    userInput(126,1);
    //prgThreeRH[5]
  }
  else if (used.item == RHSets37) {
    userInput(127,1);
    //prgThreeRH[6]
  }
  else if (used.item == RHSets38) {
    userInput(128,1);
    //prgThreeRH[7]
  }
  else if (used.item == RHSets41) {    //RH 4
    userInput(131,1);
    //prgFourRH[0]
  }
  else if (used.item == RHSets42) {
    userInput(132,1);
    //prgFourRH[1]
  }
  else if (used.item == RHSets43) {
    userInput(133,1);
    //prgFourRH[2]
  }
  else if (used.item == RHSets44) {
    userInput(134,1);
    //prgFourRH[3]
  }
  else if (used.item == RHSets45) {
    userInput(135,1);
    //prgFourRH[4]
  }
  else if (used.item == RHSets46) {
    userInput(136,1);
    //prgFourRH[5]
  }
  else if (used.item == RHSets47) {
    userInput(137,1);
    //prgFourRH[6]
  }
  else if (used.item == RHSets48) {
    userInput(138,1);
    //prgFourRH[7]
  }
  else if (used.item == RHSets51) {    //RH 5
    userInput(141,1);
    //prgFiveRH[0]
  }
  else if (used.item == RHSets52) {
    userInput(142,1);
    //prgFiveRH[1]
  }
  else if (used.item == RHSets53) {
    userInput(143,1);
    //prgFiveRH[2]
  }
  else if (used.item == RHSets54) {
    userInput(144,1);
    //prgFiveRH[3]
  }
  else if (used.item == RHSets55) {
    userInput(145,1);
    //prgFiveRH[4]
  }
  else if (used.item == RHSets56) {
    userInput(146,1);
    //prgFiveRH[5]
  }
  else if (used.item == RHSets57) {
    userInput(147,1);
    //prgFiveRH[6]
  }
  else if (used.item == RHSets58) {
    userInput(148,1);
    //prgFiveRH[7]
  }
  else if (used.item == TempSets11) {    //Temps 1
    userInput(151,1);
    //prgOneTemp[0]
  }
  else if (used.item == TempSets12) {
    userInput(152,1);
    //prgOneTemp[1]
  }
  else if (used.item == TempSets13) {
    userInput(153,1);
    //prgOneTemp[2]
  }
  else if (used.item == TempSets14) {
    userInput(154,1);
    //prgOneTemp[3]
  }
  else if (used.item == TempSets15) {
    userInput(155,1);
    //prgOneTemp[4]
  }
  else if (used.item == TempSets16) {
    userInput(156,1);
    //prgOneTemp[5]
  }
  else if (used.item == TempSets17) {
    userInput(157,1);
    //prgOneTemp[6]
  }
  else if (used.item == TempSets18) {
    userInput(158,1);
    //prgOneTemp[7]
  }
  else if (used.item == TempSets21) {    //Temps 2
    userInput(161,1);
    //prgTwoTemp[0]
  }
  else if (used.item == TempSets22) {
    userInput(162,1);
    //prgTwoTemp[1]
  }
  else if (used.item == TempSets23) {
    userInput(163,1);
    //prgTwoTemp[2]
  }
  else if (used.item == TempSets24) {
    userInput(164,1);
    //prgTwoTemp[3]
  }
  else if (used.item == TempSets25) {
    userInput(165,1);
    //prgTwoTemp[4]
  }
  else if (used.item == TempSets26) {
    userInput(166,1);
    //prgTwoTemp[5]
  }
  else if (used.item == TempSets27) {
    userInput(167,1);
    //prgTwoTemp[6]
  }
  else if (used.item == TempSets28) {
    userInput(168,1);
    //prgTwoTemp[7]
  }
  else if (used.item == TempSets31) {    //Temps 3
    userInput(171,1);
    //prgThreeTemp[0]
  }
  else if (used.item == TempSets32) {
    userInput(172,1);
    //prgThreeTemp[1]
  }
  else if (used.item == TempSets33) {
    userInput(173,1);
    //prgThreeTemp[2]
  }
  else if (used.item == TempSets34) {
    userInput(174,1);
    //prgThreeTemp[3]
  }
  else if (used.item == TempSets35) {
    userInput(175,1);
    //prgThreeTemp[4]
  }
  else if (used.item == TempSets36) {
    userInput(176,1);
    //prgThreeTemp[5]
  }
  else if (used.item == TempSets37) {
    userInput(177,1);
    //prgThreeTemp[6]
  }
  else if (used.item == TempSets38) {
    userInput(178,1);
    //prgThreeTemp[7]
  }
  else if (used.item == TempSets41) {    //Temps 4
    userInput(181,1);
    //prgFourTemp[0]
  }
  else if (used.item == TempSets42) {
    userInput(182,1);
    //prgFourTemp[1]
  }
  else if (used.item == TempSets43) {
    userInput(183,1);
    //prgFourTemp[2]
  }
  else if (used.item == TempSets44) {
    userInput(184,1);
    //prgFourTemp[3]
  }
  else if (used.item == TempSets45) {
    userInput(185,1);
    //prgFourTemp[4]
  }
  else if (used.item == TempSets46) {
    userInput(186,1);
    //prgFourTemp[5]
  }
  else if (used.item == TempSets47) {
    userInput(187,1);
    //prgFourTemp[6]
  }
  else if (used.item == TempSets48) {
    userInput(188,1);
    //prgFourTemp[7]
  }
  else if (used.item == TempSets51) {    //Temps 5
    userInput(191,1);
    //prgFiveTemp[0]
  }
  else if (used.item == TempSets52) {
    userInput(192,1);
    //prgFiveTemp[1]
  }
  else if (used.item == TempSets53) {
    userInput(193,1);
    //prgFiveTemp[2]
  }
  else if (used.item == TempSets54) {
    userInput(194,1);
    //prgFiveTemp[3]
  }
  else if (used.item == TempSets55) {
    userInput(195,1);
    //prgFiveTemp[4]
  }
  else if (used.item == TempSets56) {
    userInput(196,1);
    //prgFiveTemp[5]
  }
  else if (used.item == TempSets57) {
    userInput(197,1);
    //prgFiveTemp[6]
  }
  else if (used.item == TempSets58) {
    userInput(198,1);
    //prgFiveTemp[7]
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
    lcd.print(F("PID settings"));
    lcd.setCursor(0, 2);
    lcd.print(F("   P     I     D"));
    lcd.setCursor(2, 3);
    lcd.print(heKp);
    lcd.setCursor(8, 3);
    lcd.print(heKi);
    lcd.setCursor(14, 3);
    lcd.print(heKd);
  }
  else if (changed.to.getName() == prgCo) {
    lcd.setCursor(0, 0);
    lcd.print(F("PID settings"));
    lcd.setCursor(0, 2);
    lcd.print(F("   P     I     D"));
    lcd.setCursor(2, 3);
    lcd.print(coKp);
    lcd.setCursor(8, 3);
    lcd.print(coKi);
    lcd.setCursor(14, 3);
    lcd.print(coKd);
  }
  else if (changed.to.getName() == prgHu) {
    lcd.setCursor(0, 0);
    lcd.print(F("PID settings"));
    lcd.setCursor(0, 2);
    lcd.print(F("   P     I     D"));
    lcd.setCursor(2, 3);
    lcd.print(huKp);
    lcd.setCursor(8, 3);
    lcd.print(huKi);
    lcd.setCursor(14, 3);
    lcd.print(huKd);
  }
  else if (changed.to.getName() == prgDe) {
    lcd.setCursor(0, 0);
    lcd.print(F("PID settings"));
    lcd.setCursor(0, 2);
    lcd.print(F("   P     I     D"));
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
    lcd.print(F("Build "));
    lcd.print(Version);
    lcd.setCursor(0, 2);
    lcd.print(F("By Chris Davies"));
    lcd.setCursor(0, 3);
    lcd.print(F("siu07crd@gmail.com"));
  }
  else if (changed.to.getName() == Phe) {
    lcd.clear();
    lcd.print(F("Heater P: "));
    lcd.print(heKp);
  }
  else if (changed.to.getName() == Ihe) {
    lcd.clear();
    lcd.print(F("Heater I: "));
    lcd.print(heKi);
  }
  else if (changed.to.getName() == Dhe) {
    lcd.clear();
    lcd.print(F("Heater D: "));
    lcd.print(heKd);
  }
  else if (changed.to.getName() == Pco) {
    lcd.clear();
    lcd.print(F("Cooler P: "));
    lcd.print(coKp);
  }
  else if (changed.to.getName() == Ico) {
    lcd.clear();
    lcd.print(F("Cooler I: "));
    lcd.print(coKi);
  }
  else if (changed.to.getName() == Dco) {
    lcd.clear();
    lcd.print(F("Cooler D: "));
    lcd.print(coKd);
  }
  else if (changed.to.getName() == Phu) {
    lcd.clear();
    lcd.print(F("Humidifier P: "));
    lcd.print(huKp);
  }
  else if (changed.to.getName() == Ihu) {
    lcd.clear();
    lcd.print(F("Humidifier I: "));
    lcd.print(huKi);
  }
  else if (changed.to.getName() == Dhu) {
    lcd.clear();
    lcd.print(F("Humidifier D: "));
    lcd.print(huKd);
  }
  else if (changed.to.getName() == Pde) {
    lcd.clear();
    lcd.print(F("Dehumidifier P: "));
    lcd.print(deKp);
  }
  else if (changed.to.getName() == Ide) {
    lcd.clear();
    lcd.print(F("Dehumidifier I: "));
    lcd.print(deKi);
  }
  else if (changed.to.getName() == Dde) {
    lcd.clear();
    lcd.print(F("Dehumidifier D: "));
    lcd.print(deKd);
  }
  else if (changed.to.getName() == hourSets11) {    //hours 1
    lcd.setCursor(0, 2);
    lcd.print(prgOne[0]);
  }
  else if (changed.to.getName() == hourSets12) {
    lcd.setCursor(0, 2);
    lcd.print(prgOne[1]);
  }
  else if (changed.to.getName() == hourSets13) {
    lcd.setCursor(0, 2);
    lcd.print(prgOne[2]);
  }
  else if (changed.to.getName() == hourSets14) {
    lcd.setCursor(0, 2);
    lcd.print(prgOne[3]);
  }
  else if (changed.to.getName() == hourSets15) {
    lcd.setCursor(0, 2);
    lcd.print(prgOne[4]);
  }
  else if (changed.to.getName() == hourSets16) {
    lcd.setCursor(0, 2);
    lcd.print(prgOne[5]);
  }
  else if (changed.to.getName() == hourSets17) {
    lcd.setCursor(0, 2);
    lcd.print(prgOne[6]);
  }
  else if (changed.to.getName() == hourSets18) {
    lcd.setCursor(0, 2);
    lcd.print(prgOne[7]);
  }
  else if (changed.to.getName() == hourSets21) {
    lcd.setCursor(0, 2);
    lcd.print(prgTwo[0]);
  }
  else if (changed.to.getName() == hourSets22) {    //hours 2
    lcd.setCursor(0, 2);
    lcd.print(prgTwo[1]);
  }
  else if (changed.to.getName() == hourSets23) {
    lcd.setCursor(0, 2);
    lcd.print(prgTwo[2]);
  }
  else if (changed.to.getName() == hourSets24) {
    lcd.setCursor(0, 2);
    lcd.print(prgTwo[3]);
  }
  else if (changed.to.getName() == hourSets25) {
    lcd.setCursor(0, 2);
    lcd.print(prgTwo[4]);
  }
  else if (changed.to.getName() == hourSets26) {
    lcd.setCursor(0, 2);
    lcd.print(prgTwo[5]);
  }
  else if (changed.to.getName() == hourSets27) {
    lcd.setCursor(0, 2);
    lcd.print(prgTwo[6]);
  }
  else if (changed.to.getName() == hourSets28) {
    lcd.setCursor(0, 2);
    lcd.print(prgTwo[7]);
  }
  else if (changed.to.getName() == hourSets31) {    //hours 3
    lcd.setCursor(0, 2);
    lcd.print(prgThree[0]);
  }
  else if (changed.to.getName() == hourSets32) {
    lcd.setCursor(0, 2);
    lcd.print(prgThree[1]);
  }
  else if (changed.to.getName() == hourSets33) {
    lcd.setCursor(0, 2);
    lcd.print(prgThree[2]);
  }
  else if (changed.to.getName() == hourSets34) {
    lcd.setCursor(0, 2);
    lcd.print(prgThree[3]);
  }
  else if (changed.to.getName() == hourSets35) {
    lcd.setCursor(0, 2);
    lcd.print(prgThree[4]);
  }
  else if (changed.to.getName() == hourSets36) {
    lcd.setCursor(0, 2);
    lcd.print(prgThree[5]);
  }
  else if (changed.to.getName() == hourSets37) {
    lcd.setCursor(0, 2);
    lcd.print(prgThree[6]);
  }
  else if (changed.to.getName() == hourSets38) {
    lcd.setCursor(0, 2);
    lcd.print(prgThree[7]);
  }
  else if (changed.to.getName() == hourSets41) {    //hours 4
    lcd.setCursor(0, 2);
    lcd.print(prgFour[0]);
  }
  else if (changed.to.getName() == hourSets42) {
    lcd.setCursor(0, 2);
    lcd.print(prgFour[1]);
  }
  else if (changed.to.getName() == hourSets43) {
    lcd.setCursor(0, 2);
    lcd.print(prgFour[2]);
  }
  else if (changed.to.getName() == hourSets44) {
    lcd.setCursor(0, 2);
    lcd.print(prgFour[3]);
  }
  else if (changed.to.getName() == hourSets45) {
    lcd.setCursor(0, 2);
    lcd.print(prgFour[4]);
  }
  else if (changed.to.getName() == hourSets46) {
    lcd.setCursor(0, 2);
    lcd.print(prgFour[5]);
  }
  else if (changed.to.getName() == hourSets47) {
    lcd.setCursor(0, 2);
    lcd.print(prgFour[6]);
  }
  else if (changed.to.getName() == hourSets48) {
    lcd.setCursor(0, 2);
    lcd.print(prgFour[7]);
  }
  else if (changed.to.getName() == hourSets51) {    //hours 5
    lcd.setCursor(0, 2);
    lcd.print(prgFive[0]);
  }
  else if (changed.to.getName() == hourSets52) {
    lcd.setCursor(0, 2);
    lcd.print(prgFive[1]);
  }
  else if (changed.to.getName() == hourSets53) {
    lcd.setCursor(0, 2);
    lcd.print(prgFive[2]);
  }
  else if (changed.to.getName() == hourSets54) {
    lcd.setCursor(0, 2);
    lcd.print(prgFive[3]);
  }
  else if (changed.to.getName() == hourSets55) {
    lcd.setCursor(0, 2);
    lcd.print(prgFive[4]);
  }
  else if (changed.to.getName() == hourSets56) {
    lcd.setCursor(0, 2);
    lcd.print(prgFive[5]);
  }
  else if (changed.to.getName() == hourSets57) {
    lcd.setCursor(0, 2);
    lcd.print(prgFive[6]);
  }
  else if (changed.to.getName() == hourSets58) {
    lcd.setCursor(0, 2);
    lcd.print(prgFive[7]);
  }
  else if (changed.to.getName() == RHSets11) {    //RH 1
    lcd.setCursor(0, 2);
    lcd.print(prgOneRH[0]);
  }
  else if (changed.to.getName() == RHSets12) {
    lcd.setCursor(0, 2);
    lcd.print(prgOneRH[1]);
  }
  else if (changed.to.getName() == RHSets13) {
    lcd.setCursor(0, 2);
    lcd.print(prgOneRH[2]);
  }
  else if (changed.to.getName() == RHSets14) {
    lcd.setCursor(0, 2);
    lcd.print(prgOneRH[3]);
  }
  else if (changed.to.getName() == RHSets15) {
    lcd.setCursor(0, 2);
    lcd.print(prgOneRH[4]);
  }
  else if (changed.to.getName() == RHSets16) {
    lcd.setCursor(0, 2);
    lcd.print(prgOneRH[5]);
  }
  else if (changed.to.getName() == RHSets17) {
    lcd.setCursor(0, 2);
    lcd.print(prgOneRH[6]);
  }
  else if (changed.to.getName() == RHSets18) {
    lcd.setCursor(0, 2);
    lcd.print(prgOneRH[7]);
  }
  else if (changed.to.getName() == RHSets21) {
    lcd.setCursor(0, 2);
    lcd.print(prgTwoRH[0]);
  }
  else if (changed.to.getName() == RHSets22) {    //RH 2
    lcd.setCursor(0, 2);
    lcd.print(prgTwoRH[1]);
  }
  else if (changed.to.getName() == RHSets23) {
    lcd.setCursor(0, 2);
    lcd.print(prgTwoRH[2]);
  }
  else if (changed.to.getName() == RHSets24) {
    lcd.setCursor(0, 2);
    lcd.print(prgTwoRH[3]);
  }
  else if (changed.to.getName() == RHSets25) {
    lcd.setCursor(0, 2);
    lcd.print(prgTwoRH[4]);
  }
  else if (changed.to.getName() == RHSets26) {
    lcd.setCursor(0, 2);
    lcd.print(prgTwoRH[5]);
  }
  else if (changed.to.getName() == RHSets27) {
    lcd.setCursor(0, 2);
    lcd.print(prgTwoRH[6]);
  }
  else if (changed.to.getName() == RHSets28) {
    lcd.setCursor(0, 2);
    lcd.print(prgTwoRH[7]);
  }
  else if (changed.to.getName() == RHSets31) {    //RH 3
    lcd.setCursor(0, 2);
    lcd.print(prgThreeRH[0]);
  }
  else if (changed.to.getName() == RHSets32) {
    lcd.setCursor(0, 2);
    lcd.print(prgThreeRH[1]);
  }
  else if (changed.to.getName() == RHSets33) {
    lcd.setCursor(0, 2);
    lcd.print(prgThreeRH[2]);
  }
  else if (changed.to.getName() == RHSets34) {
    lcd.setCursor(0, 2);
    lcd.print(prgThreeRH[3]);
  }
  else if (changed.to.getName() == RHSets35) {
    lcd.setCursor(0, 2);
    lcd.print(prgThreeRH[4]);
  }
  else if (changed.to.getName() == RHSets36) {
    lcd.setCursor(0, 2);
    lcd.print(prgThreeRH[5]);
  }
  else if (changed.to.getName() == RHSets37) {
    lcd.setCursor(0, 2);
    lcd.print(prgThreeRH[6]);
  }
  else if (changed.to.getName() == RHSets38) {
    lcd.setCursor(0, 2);
    lcd.print(prgThreeRH[7]);
  }
  else if (changed.to.getName() == RHSets41) {    //RH 4
    lcd.setCursor(0, 2);
    lcd.print(prgFourRH[0]);
  }
  else if (changed.to.getName() == RHSets42) {
    lcd.setCursor(0, 2);
    lcd.print(prgFourRH[1]);
  }
  else if (changed.to.getName() == RHSets43) {
    lcd.setCursor(0, 2);
    lcd.print(prgFourRH[2]);
  }
  else if (changed.to.getName() == RHSets44) {
    lcd.setCursor(0, 2);
    lcd.print(prgFourRH[3]);
  }
  else if (changed.to.getName() == RHSets45) {
    lcd.setCursor(0, 2);
    lcd.print(prgFourRH[4]);
  }
  else if (changed.to.getName() == RHSets46) {
    lcd.setCursor(0, 2);
    lcd.print(prgFourRH[5]);
  }
  else if (changed.to.getName() == RHSets47) {
    lcd.setCursor(0, 2);
    lcd.print(prgFourRH[6]);
  }
  else if (changed.to.getName() == RHSets48) {
    lcd.setCursor(0, 2);
    lcd.print(prgFourRH[7]);
  }
  else if (changed.to.getName() == RHSets51) {    //RH 5
    lcd.setCursor(0, 2);
    lcd.print(prgFiveRH[0]);
  }
  else if (changed.to.getName() == RHSets52) {
    lcd.setCursor(0, 2);
    lcd.print(prgFiveRH[1]);
  }
  else if (changed.to.getName() == RHSets53) {
    lcd.setCursor(0, 2);
    lcd.print(prgFiveRH[2]);
  }
  else if (changed.to.getName() == RHSets54) {
    lcd.setCursor(0, 2);
    lcd.print(prgFiveRH[3]);
  }
  else if (changed.to.getName() == RHSets55) {
    lcd.setCursor(0, 2);
    lcd.print(prgFiveRH[4]);
  }
  else if (changed.to.getName() == RHSets56) {
    lcd.setCursor(0, 2);
    lcd.print(prgFiveRH[5]);
  }
  else if (changed.to.getName() == RHSets57) {
    lcd.setCursor(0, 2);
    lcd.print(prgFiveRH[6]);
  }
  else if (changed.to.getName() == RHSets58) {
    lcd.setCursor(0, 2);
    lcd.print(prgFiveRH[7]);
  }
  else if (changed.to.getName() == TempSets11) {    //Temps 1
    lcd.setCursor(0, 2);
    lcd.print(prgOneTemp[0]);
  }
  else if (changed.to.getName() == TempSets12) {
    lcd.setCursor(0, 2);
    lcd.print(prgOneTemp[1]);
  }
  else if (changed.to.getName() == TempSets13) {
    lcd.setCursor(0, 2);
    lcd.print(prgOneTemp[2]);
  }
  else if (changed.to.getName() == TempSets14) {
    lcd.setCursor(0, 2);
    lcd.print(prgOneTemp[3]);
  }
  else if (changed.to.getName() == TempSets15) {
    lcd.setCursor(0, 2);
    lcd.print(prgOneTemp[4]);
  }
  else if (changed.to.getName() == TempSets16) {
    lcd.setCursor(0, 2);
    lcd.print(prgOneTemp[5]);
  }
  else if (changed.to.getName() == TempSets17) {
    lcd.setCursor(0, 2);
    lcd.print(prgOneTemp[6]);
  }
  else if (changed.to.getName() == TempSets18) {
    lcd.setCursor(0, 2);
    lcd.print(prgOneTemp[7]);
  }
  else if (changed.to.getName() == TempSets21) {
    lcd.setCursor(0, 2);
    lcd.print(prgTwoTemp[0]);
  }
  else if (changed.to.getName() == TempSets22) {    //Temps 2
    lcd.setCursor(0, 2);
    lcd.print(prgTwoTemp[1]);
  }
  else if (changed.to.getName() == TempSets23) {
    lcd.setCursor(0, 2);
    lcd.print(prgTwoTemp[2]);
  }
  else if (changed.to.getName() == TempSets24) {
    lcd.setCursor(0, 2);
    lcd.print(prgTwoTemp[3]);
  }
  else if (changed.to.getName() == TempSets25) {
    lcd.setCursor(0, 2);
    lcd.print(prgTwoTemp[4]);
  }
  else if (changed.to.getName() == TempSets26) {
    lcd.setCursor(0, 2);
    lcd.print(prgTwoTemp[5]);
  }
  else if (changed.to.getName() == TempSets27) {
    lcd.setCursor(0, 2);
    lcd.print(prgTwoTemp[6]);
  }
  else if (changed.to.getName() == TempSets28) {
    lcd.setCursor(0, 2);
    lcd.print(prgTwoTemp[7]);
  }
  else if (changed.to.getName() == TempSets31) {    //Temps 3
    lcd.setCursor(0, 2);
    lcd.print(prgThreeTemp[0]);
  }
  else if (changed.to.getName() == TempSets32) {
    lcd.setCursor(0, 2);
    lcd.print(prgThreeTemp[1]);
  }
  else if (changed.to.getName() == TempSets33) {
    lcd.setCursor(0, 2);
    lcd.print(prgThreeTemp[2]);
  }
  else if (changed.to.getName() == TempSets34) {
    lcd.setCursor(0, 2);
    lcd.print(prgThreeTemp[3]);
  }
  else if (changed.to.getName() == TempSets35) {
    lcd.setCursor(0, 2);
    lcd.print(prgThreeTemp[4]);
  }
  else if (changed.to.getName() == TempSets36) {
    lcd.setCursor(0, 2);
    lcd.print(prgThreeTemp[5]);
  }
  else if (changed.to.getName() == TempSets37) {
    lcd.setCursor(0, 2);
    lcd.print(prgThreeTemp[6]);
  }
  else if (changed.to.getName() == TempSets38) {
    lcd.setCursor(0, 2);
    lcd.print(prgThreeTemp[7]);
  }
  else if (changed.to.getName() == TempSets41) {    //Temps 4
    lcd.setCursor(0, 2);
    lcd.print(prgFourTemp[0]);
  }
  else if (changed.to.getName() == TempSets42) {
    lcd.setCursor(0, 2);
    lcd.print(prgFourTemp[1]);
  }
  else if (changed.to.getName() == TempSets43) {
    lcd.setCursor(0, 2);
    lcd.print(prgFourTemp[2]);
  }
  else if (changed.to.getName() == TempSets44) {
    lcd.setCursor(0, 2);
    lcd.print(prgFourTemp[3]);
  }
  else if (changed.to.getName() == TempSets45) {
    lcd.setCursor(0, 2);
    lcd.print(prgFourTemp[4]);
  }
  else if (changed.to.getName() == TempSets46) {
    lcd.setCursor(0, 2);
    lcd.print(prgFourTemp[5]);
  }
  else if (changed.to.getName() == TempSets47) {
    lcd.setCursor(0, 2);
    lcd.print(prgFourTemp[6]);
  }
  else if (changed.to.getName() == TempSets48) {
    lcd.setCursor(0, 2);
    lcd.print(prgFourTemp[7]);
  }
  else if (changed.to.getName() == TempSets51) {    //Temps 5
    lcd.setCursor(0, 2);
    lcd.print(prgFiveTemp[0]);
  }
  else if (changed.to.getName() == TempSets52) {
    lcd.setCursor(0, 2);
    lcd.print(prgFiveTemp[1]);
  }
  else if (changed.to.getName() == TempSets53) {
    lcd.setCursor(0, 2);
    lcd.print(prgFiveTemp[2]);
  }
  else if (changed.to.getName() == TempSets54) {
    lcd.setCursor(0, 2);
    lcd.print(prgFiveTemp[3]);
  }
  else if (changed.to.getName() == TempSets55) {
    lcd.setCursor(0, 2);
    lcd.print(prgFiveTemp[4]);
  }
  else if (changed.to.getName() == TempSets56) {
    lcd.setCursor(0, 2);
    lcd.print(prgFiveTemp[5]);
  }
  else if (changed.to.getName() == TempSets57) {
    lcd.setCursor(0, 2);
    lcd.print(prgFiveTemp[6]);
  }
  else if (changed.to.getName() == TempSets58) {
    lcd.setCursor(0, 2);
    lcd.print(prgFiveTemp[7]);
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
