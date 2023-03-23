#include "Motor.h"
#include "Arduino.h"
#include "IMU.h"

const int MotorPins[4][2] = {{9,8},{7,6},{3,2},{5,4}};

int speed = 0;

Motor::Motor() {
   for(int i = 0; i < 4; i++) {
      for(int j = 0; j < 2; j++) {
         pinMode(MotorPins[i][j],OUTPUT);
         analogWriteFrequency(MotorPins[i][j],1000000);
      }
   }
}

void Motor::roll(int num, int power) {
   if(power < 0) {
      analogWrite(MotorPins[num][0],abs(power));
      analogWrite(MotorPins[num][1],0);
   }
   else if (power > 0) {
      analogWrite(MotorPins[num][0],0);
      analogWrite(MotorPins[num][1],abs(power));
   }
   else {
      analogWrite(MotorPins[num][0],255);
      analogWrite(MotorPins[num][1],255);
   }
}

void Motor::free() {
   for(int i = 0; i < 4; i++) {
      analogWrite(MotorPins[i][0],0);
      analogWrite(MotorPins[i][1],0);
   }
}

void Motor::stop() {
   for(int i = 0; i < 4; i++) {
      Motor::roll(i,0);
   }
}

void Motor::turnFront() {

}

void Motor::run(int angle) {
   int MotorAngle[4] = {40, 140, 225, 315};
   float MPwrVector[4] = {0};
   float MPwrMax = 0;
   angle = 450 - angle;
   if(angle > 360) angle -= 360;
   for(int i = 0; i < 4; i++) {
      MPwrVector[i] = -sin((angle - MotorAngle[i]) * (PI / 180));
      if(MPwrMax < MPwrVector[i]) MPwrMax = MPwrVector[i];
   }
   if(MPwrMax < 1) {
      for(int i = 0; i < 4; i++) {
         MPwrVector[i] *= (1 / MPwrMax);
      }
   }

   int gy = GyroGet(), addPower = 0;

   // while(GyroGet() > 45 || GyroGet() < 315) {
   //    int power = -100;
   //    if(GyroGet() > 180) power *= -1;
   //    for(int i = 0; i < 4; i++) {
   //       Motor::roll(i,power);
   //    }
   //    if(GyroGet() < 45 || GyroGet() > 315) break;
   // }

   // 30 good
   if((gy >= 10) && (gy < 180)) addPower = -20;
   else if ((gy < 350) && (gy >= 180)) addPower = 20;
   for(int i = 0; i < 4; i++) {
      int finalPower = speed * MPwrVector[i] + addPower;
      Motor::roll(i, finalPower);
      // Serial.print(finalPower);
      // Serial.print(" ");
      // プラスとかマイナスとかじゃなくて掛け算にしてみては？？ 2022.12/29
   }
   // Serial.println("");
}