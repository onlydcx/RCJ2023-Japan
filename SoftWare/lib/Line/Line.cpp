#include "Line.h"
#include "Arduino.h"

extern bool isOnLine;
extern int threshold[16];
const int MuxPins[4] = {10,11,12,30};
extern int avoidAngle;

int threshold[16] = {0};

bool isOnLine;

float LineAngle[16] = {0.0};
int readVal[16] = {0};

int avoidAngle;

int MuxCh[16][4] = {
   {0,0,0,1},{1,0,0,1},{0,1,0,1},{1,1,0,1},
   {0,0,1,1},{1,0,1,1},{0,1,1,1},{1,1,1,1},
   {0,0,0,0},{1,0,0,0},{0,1,0,0},{1,1,0,0},
   {0,0,1,0},{1,0,1,0},{0,1,1,0},{1,1,1,0}
};

Line::Line() {
   for(int i = 0; i < 4; i++) {
      pinMode(MuxPins[i],OUTPUT);
   }
   for(int i = 0; i < 16; i++) {
      threshold[i] = 200;
      LineAngle[i] = (float)22.5 * i;
   }
}

void Line::ThUpdate() {
   Serial.println("Start");
   int time = millis();
   int max[16] = {0}, min[16] = {1023};
   while((millis() - time) < 7000) {
      for(int i = 0; i < 16; i++) {
         digitalWrite(MuxPins[0],MuxCh[i][0]);
         digitalWrite(MuxPins[1],MuxCh[i][1]);
         digitalWrite(MuxPins[2],MuxCh[i][2]);
         digitalWrite(MuxPins[3],MuxCh[i][3]);
         int val = analogRead(A10);
         if(max[i] < val) max[i] = val;
         if(min[i] > val) min[i] = val;
      }
   }
   for(int i = 0; i < 16; i++) {
      threshold[i] = min[i] + (max[i] - min[i]) / 2;
      Serial.print(threshold[i]);
      Serial.print(",");
   }
   Serial.println("");
}

void Line::check() {
   isOnLine = false;
   int hcnt = 0;
   float Vx = 0, Vy = 0;
   for(int i = 0; i < 16; i++) {
      digitalWrite(MuxPins[0],MuxCh[i][0]);
      digitalWrite(MuxPins[1],MuxCh[i][1]);
      digitalWrite(MuxPins[2],MuxCh[i][2]);
      digitalWrite(MuxPins[3],MuxCh[i][3]);
      int val = analogRead(A10);
      Serial.print(val);
      Serial.print(" ");
      if(threshold[i] <= val) {
         // Serial.print(i);
         // Serial.print(" ");
         int angle = 22.5 * i * (PI / 180);
         hcnt++;
         Vx += cos(angle);
         Vy += sin(angle);
      }
   }
   Vx *= -1;
   char tmp[64];
   sprintf(tmp,"%f %f", Vx, Vy);
   // Serial.println(tmp);
   if(hcnt > 0) isOnLine = true;
   if(Vx != 0 && Vy != 0) {
      avoidAngle = atan2(Vy,Vx) * (180 / PI) + 90;
   }
   else {
      // Serial.println("None");
   }
   // delay(10);
   Serial.println("");
}