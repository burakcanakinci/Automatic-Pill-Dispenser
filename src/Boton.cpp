#include "Boton.h"
#include "DebugUtils.h"

// Constructor
Boton::Boton()
{
  lastButtonState = 0;
  buttonState = 0;
}

// Destructor
Boton::~Boton()
{
}

// -------------------------- Implementación --------------------------
void Boton::setup(uint8_t pBoton, char boton)
{
  this->pBoton = pBoton;
  this->boton = boton;  
  pinMode(pBoton, INPUT);
}

char Boton::getBoton()
{
  char aux = '\0';
  buttonState = digitalRead(pBoton);
  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button went from off to on:

    } else {
      // if the current state is LOW then the button went from on to off:
      aux = boton;
    }
    // Delay a little bit to avoid bouncing
    delay(20);
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;

  // Si el botón no se presionó, retorna el caracter nulo '\0'
  return aux;
}