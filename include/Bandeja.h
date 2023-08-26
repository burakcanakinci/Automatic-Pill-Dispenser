#ifndef Bandeja_h
#define Bandeja_h

#include <Arduino.h>
#include "MotorPasos.h"
#include "DebugUtils.h"

class Bandeja
{
private:
    // Instanciar objeto de tipo MotorPasos para el motor de la bandeja
    MotorPasos motorBandeja;

    int pinReed;

public:
    Bandeja(int in1, int in2, int in3, int in4, int pinReed);
    ~Bandeja();

    void retornarPosInicial();
    void seleccionarPastilla(uint8_t pastilla);
    void vibrar();

    void testCorriente();
};

#endif