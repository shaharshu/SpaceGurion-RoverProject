

#include "t_MotorController.h"


//DCmotor myDCmotor = new DCmotor(0,9,10);
void setup() {
  setupMotors();
  testMotors();
   Serial.begin(9600);
}



void loop()

{
 //This is a simple test code for full 6 DC motor control 
 
  {
  int speeds[6] = {200, -150, 255, -255, 100, -100};
  setMotorsSpeed(speeds);
  Serial.println("motors moving");
  delay(3000); // Run motors for 3 seconds
  stopAllMotors();
  Serial.println("motors stopped");
  delay(2000); // Wait for 2 seconds before next command

  for (int i = 0; i < 6; i++)
      {
      int speeds_2[6];
      memcpy(speeds_2, null_speed_vector, sizeof(speeds_2));
      delay(1000);
      speeds_2[i] = 200;
      setMotorsSpeed(speeds_2);
      Serial.print("motor ");Serial.print(i);Serial.println(" moving");
      }
      stopAllMotors();
      
   
  }
  

  
 //this is a simple test code for dc motor control
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
}
*/
}