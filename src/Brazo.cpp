#include "Arduino.h"
#include "Brazo.h"

/*
    Total de pasos -> 512 = 360° (una vuelta)
    El brazo necesita recorrer 70° para llegar al fondo del recipiente

    PASOS_FONDO = 70°*512 pasos/360°
    PASOS_FONDO = 115 pasos
*/
#define PASOS_FONDO 210
#define GRADOS_FONDO (uint8_t)70

#define ABAJO 1
#define ARRIBA 0

// Constructor
// Inicializar objeto MotorPasos con "Initialization List"
Brazo::Brazo(int in1, int in2, int in3, int in4, int pinMotorNeum, int pinValvula, int pinReed) : 
    motorBrazo(in1, in2, in3, in4)
{
    this->pinMotorNeum = pinMotorNeum;
    this->pinValvula = pinValvula;
    this->pinReed = pinReed;

    // Motor neumático para levantar pastillas
    pinMode(pinMotorNeum, OUTPUT);
    // Valvula con solenoide electrico
    pinMode(pinValvula, OUTPUT);
    // Reed switch para detección de posición del brazo
    pinMode(pinReed, INPUT_PULLUP);
}

// Destructor
Brazo::~Brazo()
{
}

// -------------------------- Implementación --------------------------
void Brazo::retornarPosInicial()
{
    // HIGH = Imán alejado, LOW = Imán pegado
    while (digitalRead(pinReed) == HIGH)
    {
        motorBrazo.girar(ARRIBA, 1);
    }
    motorBrazo.apagar();
}

void Brazo::succionPastilla()
{
    retornarPosInicial();
    motorBrazo.girar(ABAJO, PASOS_FONDO);
    motorBrazo.apagar();
    comenzarSuccion(1000);
    retornarPosInicial();
}

void Brazo::comenzarSuccion(int ms)
{
    digitalWrite(pinMotorNeum, HIGH);
    digitalWrite(pinValvula, LOW);  // Cierra válvula para crear vacío
    delay(ms);
}

void Brazo::soltar()
{
    motorBrazo.girar(ABAJO, PASOS_FONDO+30);
    motorBrazo.apagar();
    detenerSuccion();
    retornarPosInicial();
}

void Brazo::detenerSuccion()
{
    digitalWrite(pinMotorNeum, LOW);
    digitalWrite(pinValvula, HIGH); // Abre válvula para ecualizar presión
    delay(100);
    digitalWrite(pinValvula, LOW);  // Regresa al estado bajo lógico

}

void Brazo::testCorriente()
{
    motorBrazo.derecha();
    digitalWrite(pinMotorNeum, HIGH);
    digitalWrite(pinValvula, HIGH);
}