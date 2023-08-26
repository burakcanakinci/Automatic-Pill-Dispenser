#ifndef Dispensador_h
#define Dispensador_h

#include <Arduino.h>
#include "Brazo.h"
#include "Bandeja.h"
#include "Pastilla.h"
#include "HC05.h"
#include "virtuabotixRTC.h"
#include "Pantalla.h"
#include "Teclado.h"
#include "Pitches.h"

#include "DebugUtils.h"

#define NUM_PASTILLAS   4

class Dispensador
{
private:
    // Instanciar objetos
    Brazo       brazo;
    Bandeja     bandeja;
    Pastilla    pastilla[NUM_PASTILLAS];
    HC05        hc05;
    Teclado     teclado;
    virtuabotixRTC reloj;
    Pantalla    pantalla;

    bool pastADispensar[NUM_PASTILLAS];
    bool playOnce;
    bool isFinishedDispensing;
    bool isRetirado;
    int missedCount;

public:
    Dispensador();
    ~Dispensador();

    void setup();
    void loop();

    void eliminarPastilla(Pastilla &pastilla);
    void eliminarPastilla(uint8_t recipiente);

    void agregarPastillas(Pastilla &pastilla, uint8_t cantidad);
    void agregarPastillas(uint8_t recipiente, uint8_t cantidad);

    void dispensarPastilla(uint8_t recipiente);
    void dispensarPastilla(uint8_t recipiente, uint8_t cantidad);
    void dispensarPastilla(Pastilla &pastilla);
    void dispensarPastillas();

    void verificarHoraPastilla();

    void playMelody(int option);
    void ledNotification();

    void verificarVaso();
    static void pastillaDetectadaISR();

    void executeCommand(char* command);

    void testCorriente();
};

#endif