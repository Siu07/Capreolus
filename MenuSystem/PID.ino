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

void process() 
{    //Calculate inputs ready for PID compute and set outputs
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
