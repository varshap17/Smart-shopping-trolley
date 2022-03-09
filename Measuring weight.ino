#include "HX711.h"  //You must have this library in your arduino library folder

#define DOUT  A0
#define CLK  A1

HX711 scale(DOUT, CLK);

//Change this calibration factor as per your load cell once it is found you many need to vary it in thousands
float calibration_factor = -96650; //-106600 worked for my 40Kg max scale setup 

//=============================================================================================
//                         SETUP
//=============================================================================================
void setup() {
  Serial.begin(9600);  
  Serial.println("Press T to tare");
  scale.set_scale(-96650);  //Calibration Factor obtained from first sketch
  scale.tare();             //Reset the scale to 0  
}

//=============================================================================================
//                         LOOP
//=============================================================================================
void loop() {
  Serial.print("Weight: ");
  Serial.print(scale.get_units(), 3);  //Up to 3 decimal points
  Serial.println(" kg"); //Change this to kg and re-adjust the calibration factor if you follow lbs

  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == 't' || temp == 'T')
      scale.tare();  //Reset the scale to zero      
  }
}
