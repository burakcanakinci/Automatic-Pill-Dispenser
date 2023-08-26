#ifndef Boton_h
#define Boton_h

#include <Arduino.h>

class Boton
{
private:
    uint8_t pBoton;  // Pin del boton
    char boton; // char correspondiente al boton presionado para identificarlo
    int buttonState, lastButtonState;

public:
    Boton();
    ~Boton();

    void setup(uint8_t pBoton, char boton);
    char getBoton(); // Retorna boton presionado
};

#endif // Boton_h