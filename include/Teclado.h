#ifndef TECLADO_H_
#define TECLADO_H_

#include "Boton.h"

#define NUM_BOTONES 6

class Teclado
{
private:
    // El teclado contiene 6 botones
    Boton boton[NUM_BOTONES];

public:
    Teclado(int right, int left, int up, int down, int ok, int back);
    ~Teclado();

    char getTecla();
};

#endif // TECLADO_H_