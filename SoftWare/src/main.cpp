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

// int goalAngle = 0;

unsigned long lastKick = 0;

int gy180() {
   int __gy = GyroGet();
   if(__gy > 180) __gy -= 360;
   return __gy;
}

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
   Serial8.begin(19200);
   Wire.begin();
   GyroInit();
   pinMode(20,OUTPUT);
   BallInit();
   // while(1) {
   //    cam.update();

   //    Serial.println(goal);
   // }
}

void loop() {
   // Serial.println(goal);

   speed = 120;
   bool iscatch = analogRead(A13) < 480;
   // line.check();
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

         if(iscatch && ((goal < 90) || (goal > 270))) {
            // cam.update();
            if(goal > 180) goal -= 360;
            if(goal < 0) {
               int gAngle = goal;
               while(abs(gAngle - gy180()) > 20) {
                  // char a[64];
                  // sprintf(a,"%d %d", gAngle, gy180());
                  Serial.println(abs(gAngle - gy180()));
                  // Serial.println("回転");
                  motor.roll(0,-100);
                  motor.roll(3,-100);
                  motor.roll(1,100);
                  motor.roll(2,100);
                  // if(abs(gAngle - gy180()) < 10) {
                  //    // Serial.println("キック");
                  //    kick();
                  //    break;
                  // }
               }
               kick();
            }
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