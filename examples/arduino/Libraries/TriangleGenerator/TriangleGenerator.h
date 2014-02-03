/*
Copyright (c) 2014, Dust Networks.  All rights reserved.

Arduino library which generates a "triangle" signal.
  
\license See attached DN_LICENSE.txt.
*/

#ifndef TriangleGenerator_h
#define TriangleGenerator_h

#include <Arduino.h>

//=========================== defines =========================================

#define NUM_STEPS       8
#define DIRECTION_UP    1
#define DIRECTION_DOWN  0

//=========================== TriangleGenerator object ========================

class TriangleGenerator {
   public:
      //===== methods
      TriangleGenerator();
      void         nextValue(uint16_t* returnVal);
      //===== attributes
   private:
      //===== methods
      //===== attributes
      uint8_t      counter;
      uint8_t      direction;
};

#endif SmartMesh_h
