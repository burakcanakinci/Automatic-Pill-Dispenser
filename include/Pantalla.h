#ifndef Pantalla_H_
#define Pantalla_H_

#include <Arduino.h>
#include "U8g2lib.h"
#include "Pastilla.h"

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#define NO_BTN 0
#define BACK_BTN 1
#define OK_BTN 2
#define BACK_OK_BTN 3

class Pantalla
{
private:
// Variables
  // +-----------------------------------------------------+
  // | Constructor name                                    |
  // +-----------------------------------------------------+
  // | 1	Prefix	            U8G2                         |
  // | 2	Display Controller	ST7920                       |
  // | 3	Display Name	    128X64                         |
  // | 4	Buffer Size	        1, 2 or F (full frame buffer)|
  // | 5	Communication	    SW_SPI                         |
  // +-----------------------------------------------------+
  U8G2_ST7920_128X64_1_SW_SPI u8g2;

  int pinClk;
  int pinData;
  int pinCS;
  int pinReset;

  String commandStr;

// Constantes
  const uint8_t screenWidth = 128;
  const uint8_t screenHeight = 64;

// Variables para la pastilla
  Pastilla pastillaAux;
  uint8_t dosis;
  uint8_t recipiente;
  uint8_t frec;
  int cad[3];
  int primToma[2];
  uint8_t durTrat;
  uint8_t durTiempo;
  uint8_t selectorDia;
  bool diasSemana[7];
  uint8_t numSemanas;
  uint8_t numDias;
  bool diario;

  uint8_t cantidadAAgregar;

  uint8_t remainingPills[4];

// Variables para el reloj
  int hh, mm, ss;
  int dd, MM, aaaa;
  bool horaAux, fechaAux;
  int hhAux, mmAux;

// Funciones relacionadas a la impresión de pantallas
  void setTitle(const char* title, int botones);
  void setSubtitle(const char* subtitle);
  void printHora();
  void printFecha();
  void backButton();
  void okButton();
  void navButtons(int opcion);
  void opcion(const char* opcionNombre, int numOpcion);
  void variableFlechas(int x, int y, char direccion);
  void mostrarVariable(int x, int y, const char nombre[5], int var, char direccion);
  void mostrarVariable(int x, int y, const char nombre[5], char direccion);

// Funciones del backend de cada pantalla
  void menuBackend();
  void selecPastillaBackend();
  void frecuenciaBackend();
  void diasBackend();
  void semanasBackend();
  void duracionTratamientoBackend();   
  void primerTomaBackend(); 
  void dosisBackend();       
  void caducidadBackend();   
  void resultadoBackend();
  void dispensarBackend();
  void opcionesBackend();
  void agregarBackend();
  void eliminarBackend();
  void avisoBackend();

public:
  char botonPantalla;
  uint8_t pageIndex;
  bool dosisPerdida;

  Pantalla(int pinClk, int pinData, int pinCS, int pinReset);
  ~Pantalla();

  void setup();
  void loop();

// Páginas de la pantalla a mostrar
  void p1menu();
  void p2selecPastilla();
  void p3frecuencia();
  void p4dias();
  void p5duracionTratamiento();   
  void p6primerToma(); 
  void p7dosis();       
  void p8caducidad();
  void p9resultado();
  void p10dispensar();
  void p11opciones();
  void p12agregar();
  void p13eliminar();
  void pAviso();

// Funciones para el control con la pantalla
  void resetVars();
  void setPastilla();
  Pastilla getPastilla();
  char* getCommand();
  void dosisLista(int hh, int mm);

  void setTime(int hh, int mm, int ss);
  void setDate(int dd, int MM, int aaaa);

  void setRemainingPills(int p1, int p2, int p3, int p4);
};

#endif // Pantalla_H_