#ifndef HC05_H_
#define HC05_H_

#include <Arduino.h>
#include "DebugUtils.h"

class HC05
{
private:
    char ch;
    String commandStr;

public:
    HC05();
    ~HC05();

    char* getCommand();
};

#endif // HC05_H_