#ifndef MotorPasos_h
#define MotorPasos_h

#include <Arduino.h>

class MotorPasos
{
private:
  // Pines para el driver del motor
  uint8_t IN1;
  uint8_t IN2;
  uint8_t IN3;
  uint8_t IN4;

public:
  MotorPasos(uint8_t pDriver0, uint8_t pDriver1, uint8_t pDriver2, uint8_t pDriver3);
  ~MotorPasos();

  void setup();
  void apagar();
  void derecha();
  void izquierda();
  void girar(uint8_t dir, int pasos);
  void vibrar();
};
#endif