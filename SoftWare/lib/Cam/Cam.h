#pragma once

extern int goal;
extern int dis;
extern bool isNewData;
extern bool isNoGoal;

class Cam {
   public:
      Cam();
      void update();
};