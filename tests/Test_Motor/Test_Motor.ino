

#include "t_MotorController.h"


//DCmotor myDCmotor = new DCmotor(0);
void setup() {
  setupMotors();
  testMotors();
   Serial.begin(9600);
}



void loop()

{
   testMotors();
   
/*
{
 myDCmotor.setMove(true); 
 Serial.println("go");
 delay(1000); // Delay for 1000 milliseconds
 myDCmotor.stop();
 Serial.println("stop");
 delay(2000); // Delay for 1000 milliseconds
 myDCmotor.setMove(false); 
 Serial.println("back");
 delay(1000); // Delay for 1000 milliseconds
  myDCmotor.stop();
 Serial.println("stop");
 delay(2000); // Delay for 1000 milliseconds
 for (int i=0;i<255; i=i+20)
   {
   myDCmotor.setMove(true,i);
      Serial.print("go at ");
      Serial.print(i);
      Serial.println(" bit for secend");
      delay(1000); // Delay for 1000 milliseconds

   myDCmotor.stop();
      Serial.println("stop");
      delay(1000); // Delay for 1000 milliseconds

   myDCmotor.setMove(false,i);
      Serial.print("go backword at ");
      Serial.print(i);
      Serial.println(" bit for secend");
      delay(1000); // Delay for 1000 milliseconds

   myDCmotor.stop();
      Serial.println("stop");
      delay(1000); // Delay for 1000 milliseconds
   }
}*/
}