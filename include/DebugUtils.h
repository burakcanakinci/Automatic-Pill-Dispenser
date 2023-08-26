/*
DebugUtils.h - Simple debugging utilities.

__PRETTY_FUNCTION__ prints the function where the debug_print is originated

*/

#ifndef DEBUGUTILS_H
#define DEBUGUTILS_H

#define DEBUG

#ifdef DEBUG
  #define DEBUG_PRINT(...) Serial.print(__VA_ARGS__);
  #define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__);
  #define DEBUG_FUNCTION Serial.println(__PRETTY_FUNCTION__);
#else
  #define DEBUG_PRINT(...)
  #define DEBUG_PRINTLN(...)
  #define DEBUG_FUNCTION
#endif

#endif