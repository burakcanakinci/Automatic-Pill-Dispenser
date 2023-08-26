#include "HC05.h"

// Name:    GioHC05
// Pin:     1234
// Role:    0 (Slave)
// UART:    9600,1,2 

HC05::HC05()
{ 
    ch = '\0';
    commandStr = "";

    Serial1.begin(9600);
}

HC05::~HC05()
{
}

char* HC05::getCommand()
{
    if(Serial1.available())
    {
        ch = Serial1.read();
        commandStr += ch;

        if(ch == '*') // '*' marks the end of the string
        {
            int tempSize = commandStr.length() + 1;
            int allocSize = tempSize*sizeof(char);
            char* temp = (char*) malloc(allocSize); // Allocate memory and cast it to char*
            strcpy(temp, commandStr.c_str());
            commandStr = "";   // Clear string so it doesn't accumulate characters
            ch = '\0';

            return temp;
        }
    }
    return "";
}