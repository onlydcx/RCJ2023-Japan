#include "Arduino.h"
#include "Ball.h"

extern int BallAngle;
extern int BallStr;
extern bool isNoBall = false;

int BallPins[4] = {17,16,15,14};

int tmpBallStr[16] = {0};
char BallDebug[64];
int nearAngle, BallZeroQTY, BallAngle,BallStr, BallAngle_UC = 0;
bool isBallFront = (nearAngle == 4)? true: false;

float SCvector[16][2] = {0.0};

void BallInit() {
   for(int i = 0; i < 4; i++) {
      pinMode(BallPins[i],OUTPUT);
   }
   pinMode(21,INPUT);
   for(int i = 0; i < 16; i++) {
      float sensorDeg = 22.5 * (PI / 180) * i;
      SCvector[i][0] = sin(sensorDeg);
      SCvector[i][1] = cos(sensorDeg);
   }
}

int ChAddress[16][4] = {
   {0,0,0,0},{0,0,0,1},{0,1,0,1},{0,1,0,0},
   {1,0,1,0},{1,1,1,0},{1,0,0,1},{1,1,1,1},
   {0,0,1,1},{1,1,0,0},{1,0,0,0},{0,1,1,1},
   {0,0,1,0},{1,0,1,1},{0,1,1,0},{1,1,0,1}
};

void IRUpDate() {
   int __BallZeroQTY = 0;
   double VectorX = 0, VectorY = 0;
   for(int i = 0; i < 16; i++) {
      digitalWrite(BallPins[0],ChAddress[i][0]);
      digitalWrite(BallPins[1],ChAddress[i][1]);
      digitalWrite(BallPins[2],ChAddress[i][2]);
      digitalWrite(BallPins[3],ChAddress[i][3]);
      int strength = pulseIn(21,LOW,833);
      float sensorDeg = 22.5 * (PI / 180) * i;
      VectorX += SCvector[i][1] * strength;
      VectorY += SCvector[i][0] * strength;
      tmpBallStr[i] = strength;
      if(strength == 0) __BallZeroQTY++;
   }

   BallZeroQTY = __BallZeroQTY;
   VectorX *= -1, VectorY *= -1;
   float tmpBallAngle = (atan2(VectorX,VectorY) * (180 / PI)) + 180;
   if(tmpBallAngle < 0) tmpBallAngle += 360;
   BallAngle = (int)tmpBallAngle;
   if(BallAngle < 0) BallAngle += 360;
   int maxBallStr = 0, maxSensorID = 0;
   for(int i = 0; i < 16; i++) {
      if(tmpBallStr[i] > maxBallStr) {
         maxBallStr = tmpBallStr[i];
         maxSensorID = i;
      }
   }
   BallStr = tmpBallStr[maxSensorID];
   isNoBall = ((BallAngle == 0) && (maxBallStr == 0))? true: false;
   sprintf(BallDebug,"角度:%d 距離:%d",BallAngle,BallStr);
}