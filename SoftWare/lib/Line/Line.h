#pragma once

extern bool isOnLine;
extern int threshold[16];
extern int avoidAngle;

class Line {
   public:
      Line();
      void ThUpdate();
      void check();
};