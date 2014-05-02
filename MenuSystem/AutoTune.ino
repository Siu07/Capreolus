////01/04/14
////Version 0.0.7
///* ______     ___      .______   .______    _______   ______    __       __    __       _______.
//  /      |   /   \     |   _  \  |   _  \  |   ____| /  __  \  |  |     |  |  |  |     /       |
// |  ,----'  /  ^  \    |  |_)  | |  |_)  | |  |__   |  |  |  | |  |     |  |  |  |    |   (----`
// |  |      /  /_\  \   |   ___/  |      /  |   __|  |  |  |  | |  |     |  |  |  |     \   \    
// |  `----./  _____  \  |  |      |  |\  \-.|  |____ |  `--'  | |  `----.|  `--'  | .----)   |   
//  \______/__/     \__\ | _|      | _| `.__||_______| \______/  |_______| \______/  |_______/    
//                                                                                                   
//
//      _______  __  .__   __.  _______     _______   ______     ______    _______       _______.
//     |   ____||  | |  \ |  | |   ____|   |   ____| /  __  \   /  __  \  |       \     /       |
//     |  |__   |  | |   \|  | |  |__      |  |__   |  |  |  | |  |  |  | |  .--.  |   |   (----`
//     |   __|  |  | |  . `  | |   __|     |   __|  |  |  |  | |  |  |  | |  |  |  |    \   \    
//     |  |     |  | |  |\   | |  |____    |  |     |  `--'  | |  `--'  | |  '--'  |.----)   |   
//     |__|     |__| |__| \__| |_______|   |__|      \______/   \______/  |_______/ |_______/    */
//                                        
////#include <PID_v1.h>
////#include <PID_AutoTune_v0.h>
//
////takes approx 20 mins to register a change. So sample every 12 seconds for 100 samples. = 12000 milliseconds
//void autoTune()
//{
//  byte ATuneModeRemember=2;  //cancel autotune. 2=manual???
//  //double input=80, output=50, setpoint=180;
//  //double kp=2,ki=0.5,kd=2;
//  
//  double aTuneStep=25.5, aTuneNoise=1, aTuneStartValue=102; //step within 0-255 25.5 = 30c, noise above setpoint 0-255 addition, start value0-255 102 = 20c
//  unsigned int aTuneLookBack=12000;  //lookback time in milliseconds, 20mins
//  
//  boolean tuning = true;
//  unsigned long  modelTime, serialTime;
//  
//  PID myPID(&input, &output, &setpoint,kp,ki,kd, DIRECT);
//  PID_ATune aTune(&input, &output);
//  
//  //set to false to connect to the real world
//  
//  void setup()
//  {
//      //Setup the pid 
//    myPID.SetMode(AUTOMATIC);
//  
//    if(tuning)
//    {
//      tuning=false;
//      changeAutoTune();
//      tuning=true;
//    }
//    
//    serialTime = 0;
//    Serial.begin(9600);
//  
//  }
//  
//  void loop()
//  {
//  
//    unsigned long now = millis();
//  
//  
//  //pull the input in from the real world
//      input = analogRead(0);
//  
//    
//    if(tuning)
//    {
//      byte val = (aTune.Runtime());
//      if (val!=0)
//      {
//        tuning = false;
//      }
//      if(!tuning)
//      { //we're done, set the tuning parameters
//        kp = aTune.GetKp();
//        ki = aTune.GetKi();
//        kd = aTune.GetKd();
//        myPID.SetTunings(kp,ki,kd);
//        AutoTuneHelper(false);
//      }
//    }
//    else myPID.Compute();
//    analogWrite(0,output); 
//  
//    
//    //send-receive with processing if it's time
//    if(millis()>serialTime)
//    {
//      SerialReceive();
//      SerialSend();
//      serialTime+=500;
//    }
//  }
//  
//  void changeAutoTune()
//  {
//   if(!tuning)
//    {
//      //Set the output to the desired starting frequency.
//      output=aTuneStartValue;
//      aTune.SetNoiseBand(aTuneNoise); //setpoint+noiseBand
//      aTune.SetOutputStep(aTuneStep);
//      aTune.SetLookbackSec((int)aTuneLookBack); //sampletime = lookback time * 10
//      AutoTuneHelper(true);
//      tuning = true;
//    }
//    else
//    { //cancel autotune
//      aTune.Cancel();
//      tuning = false;
//      AutoTuneHelper(false);
//    }
//  }
//  
//  void AutoTuneHelper(boolean start)
//  {
//    if(start)
//      ATuneModeRemember = myPID.GetMode(); //start autotune
//    else
//      myPID.SetMode(ATuneModeRemember);  //cancel autotune
//  }
//  
//  
//  void SerialSend()
//  {
//    Serial.print("setpoint: ");Serial.print(setpoint); Serial.print(" ");
//    Serial.print("input: ");Serial.print(input); Serial.print(" ");
//    Serial.print("output: ");Serial.print(output); Serial.print(" ");
//    if(tuning){
//      Serial.println("tuning mode");
//    } else {
//      Serial.print("kp: ");Serial.print(myPID.GetKp());Serial.print(" ");
//      Serial.print("ki: ");Serial.print(myPID.GetKi());Serial.print(" ");
//      Serial.print("kd: ");Serial.print(myPID.GetKd());Serial.println();
//    }
//  }
//  
//  void SerialReceive()
//  {
//    if(Serial.available())
//    {
//     char b = Serial.read(); 
//     Serial.flush(); 
//     if((b=='1' && !tuning) || (b!='1' && tuning))changeAutoTune();
//    }
//  }
//  
//}
