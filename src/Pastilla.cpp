#include "Arduino.h"
#include "Pastilla.h"
#include "DebugUtils.h"

// Constructor
Pastilla::Pastilla()
{
    // nombre = '\0';
    dosis = 0;
    recipiente = 0;
    frecuencia = 0;
    caducidad[0] = 0;
    caducidad[1] = 0;
    caducidad[2] = 0;
    primerDosis[0] = 0;
    primerDosis[1] = 0;
    duracionTratamiento = 0;
    duracionTiempo = 0;
    diario = 0;
    dias[0] = false;
    dias[1] = false;
    dias[2] = false;
    dias[3] = false;
    dias[4] = false;
    dias[5] = false;
    dias[6] = false;
    pastillasRestantes = -1;
}

// Copy constructor
Pastilla::Pastilla(const Pastilla &p)
{
    // nombre = p.nombre;
    dosis = p.dosis;
    recipiente = p.recipiente;
    frecuencia = p.frecuencia;
    memcpy(caducidad, p.caducidad, 3*sizeof(int));
    memcpy(primerDosis, p.primerDosis, 2*sizeof(int));
    duracionTratamiento = p.duracionTratamiento;
    duracionTiempo = p.duracionTiempo;
    diario = p.diario;
    memcpy(dias, p.dias, 7*sizeof(bool));
    pastillasRestantes = 0;
}

// Destructor
Pastilla::~Pastilla()
{
}

void Pastilla::setup(
        // char* nombre,           // Nombre de la pastilla
        int dosis,          // Numero de pastillas a dispensar
        int recipiente,     // Recipiente en la bandeja
        int frecuencia,     // Cada cuánto tiempo se tiene que dispensar
        int caducidad[3],       //dd, mm, aaaa
        int primerDosis[2],     //hh, mm
        int duracionTratamiento,       
        int duracionTiempo, // Días/Semanas/Meses/Años
        bool diario,            // Opcion para marcar si es diario o no
        bool dias[7]            // Días a dispensar
    )
{
    // this->nombre = nombre;
    this->dosis = dosis;
    this->recipiente = recipiente;
    this->frecuencia = frecuencia;
    memcpy(this->caducidad, caducidad, 3*sizeof(int));
    memcpy(this->primerDosis, primerDosis, 2*sizeof(int));
    this->duracionTratamiento = duracionTratamiento;
    this->duracionTiempo = duracionTiempo;
    this->diario = diario;
    memcpy(this->dias, dias, 7*sizeof(bool));
    pastillasRestantes = 0;
}

const char* Pastilla::getNombre()
{
    // return nombre;
}

uint8_t Pastilla::getDosis()
{
    return dosis;
}

uint8_t Pastilla::getFrecuencia()
{
    return frecuencia;
}

uint8_t Pastilla::getDuracionTratamiento()
{
    if(duracionTiempo == 1)        // Dias
    {
        return duracionTratamiento;
    }
    else if(duracionTiempo == 2)   // Semanas
    {
        return duracionTratamiento*7;
    }
    else if(duracionTiempo == 3)   // Meses
    {
        return duracionTratamiento*30;
    }
    else
    {
        return 0;
    }
}

int* Pastilla::getPrimerDosis()
{
    return primerDosis;
}

uint8_t Pastilla::getRecipiente()
{
    return recipiente;
}

bool Pastilla::verificarCantidad()
{
    return pastillasRestantes >= dosis;
}

void Pastilla::agregarPastillas(uint8_t cantidad)
{
    pastillasRestantes += cantidad;
}

void Pastilla::eliminar()
{
    // nombre = '\0';
    dosis = 0;
    recipiente = 0;
    frecuencia = 0;
    caducidad[0] = 0;
    caducidad[1] = 0;
    caducidad[2] = 0;
    primerDosis[0] = 0;
    primerDosis[1] = 0;
    duracionTratamiento = 0;
    duracionTiempo = 0;
    diario = 0;
    dias[0] = false;
    dias[1] = false;
    dias[2] = false;
    dias[3] = false;
    dias[4] = false;
    dias[5] = false;
    dias[6] = false;
    pastillasRestantes = -1;
}

void Pastilla::getPillInfo()
{   
    if(recipiente == 0)
    {
        return;
    }

    
    // DEBUG_PRINT(" Nombre: "); DEBUG_PRINTLN(nombre);
    DEBUG_PRINT(" Recipiente: "); DEBUG_PRINTLN(recipiente);
    DEBUG_PRINT(" Dosis: "); DEBUG_PRINTLN(dosis);
    DEBUG_PRINT(" Frecuencia: cada "); DEBUG_PRINT(frecuencia); DEBUG_PRINTLN(" minutos")
    // DEBUG_PRINT(" Caducidad (dd/MM/aaaa): "); DEBUG_PRINT(caducidad[0]); DEBUG_PRINT("/"); DEBUG_PRINT(caducidad[1]); DEBUG_PRINT("/"); DEBUG_PRINTLN(caducidad[2]);
    DEBUG_PRINT(" Primera toma (hh:mm): "); DEBUG_PRINT(primerDosis[0]); DEBUG_PRINT(":"); DEBUG_PRINTLN(primerDosis[1]);
    DEBUG_PRINT(" Pastillas restantes: "); DEBUG_PRINTLN(pastillasRestantes); DEBUG_PRINTLN("");
    // DEBUG_PRINT(" Duracion del tratamiento: "); DEBUG_PRINT(duracionTiempo);
    // if(diario)
    // {
    //     DEBUG_PRINTLN(" dias");
    // }
    // else
    // {
    //     DEBUG_PRINTLN(" semanas");
    //     for (int i = 0; i < 7; i++)
    //     {
    //         DEBUG_PRINT("Dia "); DEBUG_PRINT(i+1);

    //         if (dias[i])
    //         {
    //             DEBUG_PRINTLN(" Sí")
    //         }
    //         else
    //         {
    //             DEBUG_PRINTLN(" No")
    //         }
    //     }
    // }
}