#include "Arduino.h"
#include "Bandeja.h"


/*
    Total de pasos -> 512 = 360° (una vuelta)
    Se tienen 5 recipientes -> 360°/5 = 72° por recipiente

    PASOS_RECIPIENTE = 72°*512 pasos/360°
    PASOS_RECIPIENTE = 102.4 pasos
*/
#define PASOS_RECIPIENTE 103
#define GRADOS_RECIPIENTE (uint16_t)72

#define DERECHA 1
#define IZQUIERDA 0

// Constructor
// Inicializar objeto de tipo MotorPasos con "Initialization List"
Bandeja::Bandeja(int in1, int in2, int in3, int in4, int pinReed) : 
    motorBandeja(in1, in2, in3, in4)
{
    this->pinReed = pinReed;
    // Reed switch para detección de posición de la bandeja
    pinMode(pinReed, INPUT_PULLUP);
}

// Destructor
Bandeja::~Bandeja()
{
}

// -------------------------- Implementación --------------------------
void Bandeja::retornarPosInicial()
{
    // HIGH = Imán alejado, LOW = Imán pegado
    while (digitalRead(pinReed) == HIGH)
    {
        motorBandeja.girar(DERECHA, 1);
    }
    // motorBandeja.girar(DERECHA, 10);
    motorBandeja.apagar();
}

void Bandeja::seleccionarPastilla(uint8_t pastilla)
{
    motorBandeja.girar(DERECHA, pastilla * PASOS_RECIPIENTE);
    motorBandeja.apagar();
}

void Bandeja::vibrar()
{
    motorBandeja.vibrar();
    motorBandeja.apagar();
}

void Bandeja::testCorriente()
{
    motorBandeja.derecha();
}