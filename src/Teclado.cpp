#include "Teclado.h"
#include "DebugUtils.h"

#define NUM_BOTONES 6

Teclado::Teclado(int right, int left, int up, int down, int ok, int back) : 
    boton({}) // Inicializa vector de botones
{
    /*
    BUTTON  VALUE
    right   [r]
    left    [l]
    up      [u]
    down    [d]
    ok      [o]
    back    [b]
    */

    boton[0].setup(right, 'r'); 
    boton[1].setup(left, 'l'); 
    boton[2].setup(up, 'u'); 
    boton[3].setup(down, 'd'); 
    boton[4].setup(ok, 'o');
    boton[5].setup(back, 'b');

}

Teclado::~Teclado()
{
}

// Verifica con un ciclo que algún botón haya sido presionado
char Teclado::getTecla()
{
    char aux = '\0';
    for (int i = 0; i < NUM_BOTONES; i++)
    {
        // Recorre el vector de botones y obtiene su estado (Presionado/Sin presionar)
        aux = boton[i].getBoton();
        if(aux != '\0')
        {
            // Si algún botón es presionado, rompe el ciclo y retorna el valor
            // DEBUG_PRINT("[KB]: ");
            // DEBUG_PRINTLN(aux);
            break;
        }
        aux = '\0';
    }

    return aux;
}