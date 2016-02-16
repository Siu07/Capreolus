/* 
 * Arduino PID AutoTune Code
 * by Brett Beauregard
 * April 2011
 
 
 * February 2013
 * Edited by Konstantine @Palanski to incorporate Autotune and PID Front End
 * Edits and additions marked with //KP
 
 *HOW TO USE:
 * Upload this code to Arduino with the appropriate pin settings and sensor settings.
 * Download Processing with the ControlP5 library and run the PID_FrontEnd_v03 (set appropriate COM)
 * myPort = new Serial(this, Serial.list()[0], 9600);  //Edit the [0] value to select appropriate COM
 * Allow the system to come to equilibrium after setting an initial setpoint
 * When system is at equilibrium, toggle Tuning to 'On'.
 * When the tuning is finished, the Tuning toggle will go to 'Off', and the new PID values will be on the screen.
 * SEARCH ALL CODE FOR //KP to see where edits MUST be made by YOU for this to be effective in the tuning
*/


//DS18B20 OneWire and DallasTemperature Library Initialization
#include <OneWire.h>
#include <DallasTemperature.h>
#define oneWireBus 8
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
//DeviceAddress tempacqua = {
//  0x28, 0x2F, 0x44, 0x48, 0x05, 0x00, 0x00, 0xB3 }; //
//DeviceAddress tempcaldaia = {
//  0x28, 0xFF, 0x41, 0x05, 0x20, 0x14, 0x00, 0xAE }; //VALORE VERO
float tempcheckcaldaia;
//PID and PID Autotune Library Initialization
#include <PID_v1.h>
#include <PID_AutoTune_v0.h>

//
byte ATuneModeRemember=2;
double Input, Output, Setpoint=40; //KP Set initial Setpoint
double kp=100,ki=0.5,kd=2; //KP Set intial PID parameters, if you have any.

double kpmodel=1.5, taup=100, theta[50];
double OutputStart=5000;

double aTuneStep=5000, aTuneNoise=1, aTuneStartValue=5000; //KP Set aTuneStep to ensure noticeable heating and cooling from aTuneStartValue
unsigned int aTuneLookBack=120; //KP How far back to look back to find maxima and minima. 
// For Slow processes this will be large, for fast processes, this will be smaller.

unsigned long  modelTime, serialTime;

PID myPID(&Input, &Output, &Setpoint,kp,ki,kd, DIRECT);
PID_ATune aTune(&Input, &Output);

//set to false to connect to the real world
boolean useSimulation = false; //KP Keep false unless simulating

//Tuning?
boolean tuning = false; //KP Keep False

//KP Relay settings. (I'm running my code with a relay).
const int relayPin = 14;
int WindowSize = 10000;
unsigned long windowStartTime;

void setup()
{ 
  windowStartTime = millis();
  pinMode(relayPin, OUTPUT); //KP Set relayPin as Output
  
  if(useSimulation)
  {
    for(byte i=0;i<50;i++)
    {
      theta[i]=OutputStart;
    }
    modelTime = 0;
  }
  
  //tell the PID to range between 0 and the full window size
  myPID.SetOutputLimits(0, WindowSize);
  
  //Setup the pid 
  myPID.SetMode(AUTOMATIC);

  if(tuning)
  {
    tuning=false;
    changeAutoTune();
    tuning=true;
  }
  
  aTune.SetControlType(1); //KP Set 0 for PI control and 1 for PID control autotuning parameters
  
  serialTime = 0;
  Serial.begin(9600);
  sensors.begin();

}

void loop()
{
 unsigned long now = millis(); 
 if(!useSimulation)
  { //pull the Input in from the real world
    sensors.requestTemperatures();
    tempcheckcaldaia= sensors.getTempCByIndex(0);
   //Pass to Input of PID
   Input = sensors.getTempCByIndex(0);
  }
  
  if(tuning)
  {
    byte val = (aTune.Runtime());
    if (val!=0)
    {
      tuning = false;
    }
    if(!tuning)
    { //we're done, set the tuning parameters
      kp = aTune.GetKp();
      ki = aTune.GetKi();
      kd = aTune.GetKd();
      myPID.SetTunings(kp,ki,kd);
      AutoTuneHelper(false);
    }
  }
   if(myPID.GetMode()==AUTOMATIC && tempcheckcaldaia >= (70)) //se il PID è ON e la temperatura caldaia maggiore del massimo
      {
        myPID.SetMode(MANUAL);             //turn the PID off
        Output=0; //spegne la caldaia
      }  

      if(myPID.GetMode()==MANUAL && tempcheckcaldaia < 65) //se il PID è OFF e la temperatura caldaia minore del massimo -5 °C
      {
        myPID.SetMode(AUTOMATIC);             //turn the PID on
      }  
  else myPID.Compute();
  
  if(useSimulation)
  {
    theta[30]=Output;
    if(now>=modelTime)
    {
      modelTime +=100; 
      DoModel();
    }
  }
  else //KP This is optimized for Relay output.
  { 
  if(now - windowStartTime>WindowSize)
  { //time to shift the Relay Window
    windowStartTime += WindowSize;
  }
  if(Output > now - windowStartTime) digitalWrite(relayPin,HIGH);
  else digitalWrite(relayPin,LOW);  
}
  
  //send-receive with processing if it's time
  if(millis()>serialTime)
  {
    SerialReceive();
    SerialSend();
    serialTime+=500;
  }
}


/********************************************
 * Serial Communication functions / helpers
 ********************************************/


union {                // This Data structure lets
  byte asBytes[24];    // us take the byte array
  float asFloat[6];    // sent from processing and
}                      // easily convert it to a
foo;                   // float array
// getting float values from processing into the arduino
// was no small task.  the way this program does it is
// as follows:
//  * a float takes up 4 bytes.  in processing, convert
//    the array of floats we want to send, into an array
//    of bytes.
//  * send the bytes to the arduino
//  * use a data structure known as a union to convert
//    the array of bytes back into an array of floats

//  the bytes coming from the arduino follow the following
//  format:
//  0: 0=Manual, 1=Auto, else = ? error ?
//  1: 0=Tuning OFF, 1=Tuning ON, else = ? error ?
//  2-5: float setpoint
//  6-9: float input
//  10-13: float output
//  14-17: float P_Param
//  18-21: float I_Param
//  22-24: float D_Param

void changeAutoTune()
{
 if(!tuning)
  {
    //Set the Output to the desired starting frequency.
    aTuneStartValue = Output; //KP Initial aTuneStartValue will be = Output at Toggle
    aTune.SetNoiseBand(aTuneNoise);
    aTune.SetOutputStep(aTuneStep);
    aTune.SetLookbackSec((int)aTuneLookBack);
    AutoTuneHelper(true);
    tuning = true;
  }
  else
  { //cancel autotune
    aTune.Cancel();
    tuning = false;
    AutoTuneHelper(false);
  }
}

void AutoTuneHelper(boolean start)
{
  if(start)
    ATuneModeRemember = myPID.GetMode();
  else
    myPID.SetMode(ATuneModeRemember);
}


void SerialSend()
{
Serial.print("PID ");
  Serial.print(Setpoint);   
  Serial.print(" ");
  Serial.print(Input);   
  Serial.print(" ");
  Serial.print(Output);   
  Serial.print(" ");
  Serial.print(myPID.GetKp());   
  Serial.print(" ");
  Serial.print(myPID.GetKi());   
  Serial.print(" ");
  Serial.print(myPID.GetKd());   
  Serial.print(" ");
  if(myPID.GetMode()==AUTOMATIC) Serial.print("Automatic");
  else Serial.print("Manual");  
  Serial.print(" ");
 // if(myPID.GetDirection()==DIRECT) Serial.print("Direct");
  //else Serial.print("Reverse");
 // Serial.print(" ");
  if(tuning==false) Serial.println("Off"); //KP Added the On/Off for Tuning Toggle
  else Serial.println("On");
}

void SerialReceive()
{
 
  // read the bytes sent from Processing
  int index=0;
  
  byte Auto_Man = -1;
 // byte Direct_Reverse = -1;
  byte Tuning_Mode = -1; //KP Tuning Mode?
  
  while(Serial.available()&&index<26)
  {
    if(index==0) Auto_Man = Serial.read();
    else if(index==1) Tuning_Mode = Serial.read(); //KP Tuning Mode?
    //else if(index==1) Direct_Reverse = Serial.read();
    //else if(index==2) Tuning_Mode = Serial.read(); //KP Tuning Mode?
    else foo.asBytes[index-2] = Serial.read();
    index++;
  } 
  
  // if the information we got was in the correct format, 
  // read it into the system
  if(index==26  && (Auto_Man==0 || Auto_Man==1) && (Tuning_Mode==0 || Tuning_Mode==1))
  {
    Setpoint=double(foo.asFloat[0]);
    //Input=double(foo.asFloat[1]);       // * the user has the ability to send the 
                                          //   value of "Input"  in most cases (as 
                                          //   in this one) this is not needed.
    if(Auto_Man==0)                       // * only change the Output if we are in 
    {                                     //   manual mode.  otherwise we'll get an
      Output=double(foo.asFloat[2]);      //   Output blip, then the controller will 
    }                                     //   overwrite.
    
    double p, i, d;                       // * read in and set the controller tunings
    p = double(foo.asFloat[3]);           //
    i = double(foo.asFloat[4]);           //
    d = double(foo.asFloat[5]);           //
    myPID.SetTunings(p, i, d);            //
    
    if(Auto_Man==0) myPID.SetMode(MANUAL);// * set the controller mode
    else myPID.SetMode(AUTOMATIC);             //
    
   // if(Direct_Reverse==0) myPID.SetControllerDirection(DIRECT);// * set the controller Direction
  //  else myPID.SetControllerDirection(REVERSE);          //
    
    if(Tuning_Mode == 0) tuning=false; // Set Tuning mode on/off
    else tuning=true;
  }
  Serial.flush();                         // * clear any random data from the serial buffer
}

void DoModel()
{
  //cycle the dead time
  for(byte i=0;i<49;i++)
  {
    theta[i] = theta[i+1];
  }
  //compute the Input
  Input = (kpmodel / taup) *(theta[0]-OutputStart) + Input*(1-1/taup) + ((float)random(-10,10))/100;

}
