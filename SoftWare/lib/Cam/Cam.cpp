#include "Cam.h"
#include "Arduino.h"

extern int goal;
extern bool isNewData;

int goal = 0;
bool isNewData;

String cmds[3] = {"\0"};

Cam::Cam() {
   Serial8.begin(115200);
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

int Cam::update(){
   if(Serial8.available()) {
      String byteRead = Serial8.readStringUntil('\0');
      goal = byteRead.toInt();
   }
}