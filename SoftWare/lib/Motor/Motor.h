#pragma once

extern int speed;

class Motor {
   public:
      Motor();
      void run(int angle);
      void roll(int num, int speed);
      void stop();
      void free();
      void turnFront();
};