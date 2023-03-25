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

unsigned long lastKick = 0;

bool isBlue = 0;

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
      delay(150);
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
   speed = 150;
   kick();
}

void loop() {
   IRUpDate();
   bool isNear = (BallStr > 500) && ((BallAngle < 15) || (BallAngle > 345));
   bool iscatch = analogRead(A13) < 80;
   line.check();
   cam.update();

   if(isOnLine) {
      motor.run(avoidAngle);
      delay(30);
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
         else dribble(0);

         if(iscatch) {
            if((goal < 20) || (goal > 350)) {
               dribble(1);
               delay(100);
               kick();
            }
            else if((goal < 45) || (goal > 300)) {
               dribble(2);
               int rollPower = 60;
               int time = millis();
               if(goal > 180) goal -= 360;
               if(goal < 0) {
                  while((abs(goal - gy180()) > 10) && ((millis() - time) < 700)) {
                     motor.roll(0,-rollPower);
                     motor.roll(1,-rollPower);
                     motor.roll(2,-rollPower);
                     motor.roll(3,-rollPower);
                  }
               }
               else {
                  while((abs(goal - gy180()) > 10) && ((millis() - time) < 700)) {
                     motor.roll(0,rollPower);
                     motor.roll(1,rollPower);
                     motor.roll(2,rollPower);
                     motor.roll(3,rollPower);
                  }
               }
               dribble(1);
               delay(100);
               kick();
               dribble(0);
            }
            else {
               if(((goal >= 45) && (goal < 90)) || ((goal <= 300) && (goal > 270))) {
                  dribble(2);
                  int time = millis();
                  while(((millis() - time) < 1500) && iscatch) {
                     cam.update();
                     motor.run(180);
                     if((goal < 45) || (goal > 315)) {
                        break;
                     }
                  }
               }
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
                  toMove = BallAngle + dirPlus * 1.1;
               }
               else {
                  toMove = BallAngle - dirPlus * 1.1;
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