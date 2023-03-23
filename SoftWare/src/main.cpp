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

void dribble(int mode) {
   int pulse[3] = {1000, 1400, 2000};
   esc.writeMicroseconds(pulse[mode]);
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
         if(isNear) dribble(1);

         if(iscatch && ((goal < 90) || (goal > 270))) {
            dribble(2);
            int rollPower = 100;
            if(goal > 180) goal -= 360;
            if(goal < 0) {
               while(abs(goal - gy180()) > 20) {
                  motor.roll(0,-rollPower);
                  motor.roll(1,rollPower);
                  motor.roll(2,rollPower);
                  motor.roll(3,-rollPower);
               }
               kick();
            }
            else {
               while(abs(goal - gy180()) > 20) {
                  motor.roll(0,rollPower);
                  motor.roll(1,-rollPower);
                  motor.roll(2,-rollPower);
                  motor.roll(3,rollPower);
               }
               kick();
            }
            dribble(0);
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