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

bool iscatch() {
   return (analogRead(A13) < 150);
}

int rey() {
   float distance = float(dis), angle = float(goal);
   angle = 90 - angle;
   angle *= (PI / 180);
   int y = distance * sin(angle);
   return y;
}

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
   // line.ThUpdate();
   // while(1) {}
}

void loop() {
   IRUpDate();
   bool isNear = (BallStr > 450) && ((BallAngle < 15) || (BallAngle > 345));
   line.check();
   cam.update();

   if(isOnLine) {
      motor.run(avoidAngle);
      delay(20);
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

         if(!isNoGoal) {
            while(iscatch() && !isOnLine) {
               line.check();
               cam.update();

               if(rey() > 105) {
                  dribble(2);
                  speed = 170;
                  motor.run(goal);
               }
               else {
                  speed = 150;
                  if((goal < 35) || (goal > 325)) { // 目の前
                     dribble(1);
                     kick();
                  }
                  else if((goal < 50) || (goal > 310)) { // 狙える位置
                     dribble(2);
                     cam.update(); 
                     motor.run(goal);
                     // if(goal <= 180){
                     //    motor.run(goal + 30);
                     // }else{
                     //    motor.run(goal - 30);
                     // }
                  }
                  else {
                     // if(((goal >= 45) && (goal < 90)) || ((goal <= 300) && (goal > 270))) { // 狙うには急すぎ
                        dribble(2);
                        int time = millis();
                        while(((millis() - time) < 1500)) {
                           cam.update();
                           motor.run(180);
                           if(((goal < 45) || (goal > 300)) || !iscatch()) {
                              motor.stop();
                              // delay(100);
                              break;
                           }
                        }
                     // }
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