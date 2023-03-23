#include "Arduino.h"
#include <Wire.h>
#include "Motor.h"
#include "IMU.h"
#include "Line.h"
#include "Ball.h"
#include "Cam.h"
#include "Servo.h"

#define dribbrer 37

Servo esc;

Motor motor;
Line line;
Cam cam;

// int goalAngle = 0;

unsigned long lastKick = 0;

void drribler_init() {
   pinMode(dribbrer,OUTPUT);
   esc.attach(dribbrer);
   esc.writeMicroseconds(2000);
   delay(2000);
   esc.writeMicroseconds(1000);
   delay(2000);
}

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
   drribler_init();
   speed = 120;
}

void loop() {
   IRUpDate();
   bool isNear = (BallStr > 500) && ((BallAngle < 15) || (BallAngle > 345));
   bool iscatch = analogRead(A13) < 480;
   // line.check();
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