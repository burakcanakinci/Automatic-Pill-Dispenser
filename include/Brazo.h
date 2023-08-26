#ifndef Brazo_h
#define Brazo_h

#include <Arduino.h>
#include "MotorPasos.h"
#include "DebugUtils.h"

class Brazo
{
private:
    // Instanciar objeto de tipo MotorPasos para el motor del brazo
    MotorPasos motorBrazo;

    int pinMotorNeum;
    int pinValvula;
    int pinReed;

public:
    Brazo(int in1, int in2, int in3, int in4, int pinMotorNeum, int pinValvula, int pinReed);
    ~Brazo();

    void retornarPosInicial();
    void succionPastilla();
    void comenzarSuccion(int ms);
    void detenerSuccion();
    void soltar();

    void testCorriente();
};

#endif