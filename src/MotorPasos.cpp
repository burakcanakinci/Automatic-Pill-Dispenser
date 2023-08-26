#include "Arduino.h"
#include "MotorPasos.h"
#include "DebugUtils.h"

#define RETARDO_MOTOR (uint8_t)2
#define NUM_PASOS (uint16_t)512

// Constructor
MotorPasos::MotorPasos(uint8_t pDriver0, uint8_t pDriver1, uint8_t pDriver2, uint8_t pDriver3)
{
  IN1 = pDriver0;
  IN2 = pDriver1;
  IN3 = pDriver2;
  IN4 = pDriver3;

  setup();
}

// Destructor
MotorPasos::~MotorPasos()
{
}

// -------------------------- Implementación --------------------------
void MotorPasos::setup()
{
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void MotorPasos::apagar()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void MotorPasos::girar(uint8_t dir, int pasos)
{ 
  for (int i = 0; i < pasos; i++)
  {
    if (dir == 1)
    {
      derecha();
    }
    else
    {
      izquierda();
    }
  }
}

void MotorPasos::vibrar()
{
  for (int i = 0; i < 20; i++)
  {
    for (int j = 0; j < 10; j++)
    {
      derecha();
      derecha();
    }
    for (int j = 0; j < 10; j++)
    {
      izquierda();
      izquierda();
    }
  }
}

void MotorPasos::derecha()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  delay(RETARDO_MOTOR);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(RETARDO_MOTOR);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);
  delay(RETARDO_MOTOR);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(RETARDO_MOTOR);
}

void MotorPasos::izquierda()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  delay(RETARDO_MOTOR);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(RETARDO_MOTOR);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);
  delay(RETARDO_MOTOR);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(RETARDO_MOTOR);
}