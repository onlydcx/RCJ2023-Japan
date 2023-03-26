#include "Cam.h"
#include "Arduino.h"

extern int goal;
extern bool isNewData;
extern int dis;
extern bool isNoGoal;

int goal = -1;
int dis = -1;
bool isNewData;
bool isNoGoal;

String cmds[3] = {"\0"};

Cam::Cam() {
   Serial8.begin(19200);
}

void split(String data, char delimiter, String *dst) {
   int index = 0; 
   int len = data.length();
   for (int i = 0; i < len; i++) {
      char tmp = data.charAt(i);
      if (tmp == delimiter) {
         index++;
      }
      else dst[index] += tmp;
   }
}

void Cam::update(){
   if(Serial8.available()) {
      String byteRead = Serial8.readStringUntil('\0');
      // int a = byteRead.toInt();

      String cmds[3] = {"\0"}; // 分割された文字列を格納する配列 
      int a = 0, b = 0;
      split(byteRead,' ',cmds);
      a = cmds[0].toInt();
      b = cmds[1].toInt();

      if(a > 0) {
         goal = a;
         isNoGoal = false;
      }
      else if (a < 0) {
         isNoGoal = true;
      }
      if(b > 0) {
         dis = b;
      }
   }
}