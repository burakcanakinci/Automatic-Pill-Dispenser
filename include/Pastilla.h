#ifndef Pastilla_h
#define Pastilla_h

#include <Arduino.h>

class Pastilla
{
private:
    const char* nombre; // Nombre de la pastilla
    uint8_t dosis;      // Numero de pastillas a dispensar
    uint8_t recipiente; // Recipiente en la bandeja
    uint8_t frecuencia; // Cada cuánto tiempo se tiene que dispensar
    int caducidad[3];   //dd, mm, aaaa
    int primerDosis[2]; //hh, mm
    int duracionTratamiento;       
    uint8_t duracionTiempo; // 1=dias, 2=semanas, 3=meses, 4=años
    bool diario;        // Opcion para marcar si es diario o no
    bool dias[7];       // Días a dispensar

    // Tamaño total: 18 Bytes + nombre (variable)

public:
    uint8_t pastillasRestantes;

    Pastilla();
    Pastilla(const Pastilla &p);
    ~Pastilla();

    void setup(
        // char* nombre,           // Nombre de la pastilla
        int dosis,              // Numero de pastillas a dispensar
        int recipiente,         // Recipiente en la bandeja
        int frecuencia,         // Cada cuánto tiempo se tiene que dispensar (horas)
        int caducidad[3],       //dd, mm, aaaa
        int primerDosis[2],     //hh, mm
        int duracionTratamiento,        
        int duracionTiempo,     // 1=dias, 2=semanas, 3=meses, 4=años
        bool diario,            // Opcion para marcar si es diario o no
        bool dias[7]            // Días a dispensar
    );

    const char* getNombre();
    uint8_t getDosis();
    uint8_t getRecipiente();
    uint8_t getFrecuencia();
    uint8_t getDuracionTratamiento();
    int* getPrimerDosis();

    void eliminar();
    bool verificarCantidad();
    void agregarPastillas(uint8_t cantidad);

    void getPillInfo();
};

#endif