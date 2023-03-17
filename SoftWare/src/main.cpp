#include "Arduino.h"
#include <Wire.h>
#include "Motor.h"
#include "IMU.h"
#include "Line.h"
#include "Ball.h"
#include "Cam.h"

Motor motor;
Line line;
Cam cam;

int goalAngle = 0;

unsigned long lastKick = 0;

bool kick() {
   if((millis() - lastKick) > 150) {
      digitalWrite(20,HIGH);
      delay(200);
      digitalWrite(20,LOW);
      lastKick = millis();
      return true;
   }
   else return false;
}

void setup() {
   Serial.begin(9600);
   Wire.begin();
   GyroInit();
   pinMode(20,OUTPUT);
   BallInit();
   while(1) {
      line.check();
   }
}

void loop() {

   speed = 130;
   bool iscatch = analogRead(A13) < 500;
   line.check();
   IRUpDate();
   cam.update();

   if(false) {
      motor.run(avoidAngle);
      // delay(10);
   }
   else {
      if(isNoBall) {
         motor.stop();
      }
      else {
         int dirPlus = 0;
         if (BallAngle <= 40) {
            dirPlus = BallAngle ;
         }
         else if (BallAngle >= 320) {
            dirPlus = (360 - BallAngle);
         }
         else {
            dirPlus = 50;
         }

         int toMove = 180;

         if(iscatch && ((goal < 90) || (goal > 315))) {
            if(goal < 90) {
               while((goal - GyroGet()) >= 0) {
                  motor.roll(1,100);
                  motor.roll(2,100);
                  if((goal - GyroGet()) < 0) {
                     kick();
                     break;
                  }
               }
            }
            // else {
            //    while((goal - GyroGet()) <= 0) {
            //       motor.roll(1,100);
            //       motor.roll(2,100);
            //       if((goal - GyroGet()) < 0) {
            //          kick();
            //          break;
            //       }
            //    }
            // }
         }

         if (BallStr < 300) {
            toMove = BallAngle;
         }
         else if (BallStr > 400) {
            if (BallAngle <= 5 || BallAngle >= 355) {
               toMove = BallAngle;
            }
            else {
               if (BallAngle <= 180) {
                  toMove = BallAngle + dirPlus * 1.2;
               }
               else {
                  toMove = BallAngle - dirPlus * 1.2;
               }
            }
         }
         else {
            if (BallAngle <= 180) {
               toMove = BallAngle + dirPlus;
            } 
            else {
               toMove = BallAngle - dirPlus;
            }
         }
         motor.run(toMove);
      }
   }
}