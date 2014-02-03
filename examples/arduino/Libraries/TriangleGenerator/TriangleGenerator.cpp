/*
Copyright (c) 2014, Dust Networks.  All rights reserved.

Arduino library which generates a "triangle" signal.
  
\license See attached DN_LICENSE.txt.
*/

#include "Arduino.h"
#include "TriangleGenerator.h"

//########################### public ##########################################

/**
\brief Constructor.
*/
TriangleGenerator::TriangleGenerator() {
}

/**
\brief Get the next value.
*/
void TriangleGenerator::nextValue(uint16_t* returnVal) {
   uint16_t newValue;
   
   // decide whether to go up or down
   if (counter==0) {
      direction = DIRECTION_UP;
   }
   if (counter==NUM_STEPS-1) {
      direction = DIRECTION_DOWN;
   }
   
   // calculate new value
   if (direction==DIRECTION_UP) {
      counter++;
   } else {
      counter--;
   }
   *returnVal      = 0x10000/NUM_STEPS;
   *returnVal     *= counter;
}

//########################### private #########################################
