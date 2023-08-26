#include <Arduino.h>
#include "Dispensador.h"
#include "EEPROM.h"
#include "DebugUtils.h"

/**********************************************************************************************/
// ASIGNACION DE PINES
/**********************************************************************************************/
// Pines del dispensador
#define PIN_SENSOR_PASTILLA     21  // Pin del Arduino Mega capaz de generar interrupciones
#define PINS_TECLADO            35, 37, 39, 41, 43, 45 // r, l, u, d, o, b
#define PIN_BUZZER              27
#define PIN_LED_VASO            25
#define PIN_REED_VASO           22

// Pines del brazo
#define PINS_MOTOR_BRAZO        47, 49, 51, 53
#define PIN_MOTOR_NEUM          8
#define PIN_VALVULA             9
#define PIN_REED_BRAZO          24 

// Pines de la bandeja
#define PINS_MOTOR_BAND         52, 50, 48, 46
#define PIN_REED_BAND           26

// Pines del módulo BT HC-05
// PIN_RX                       TX1
// PIN_TX                       RX1

// Pines del módulo RTC (DS1302) 
#define PINS_RTC                36, 34, 32 // SCLK, IO, CE

// Pines para la pantalla
#define CLK_PIN                 10
#define DATA_PIN                11
#define CS_PIN                  12
#define RESET_PIN               13

/**********************************************************************************************/
// CONSTANTES
/**********************************************************************************************/
#define NOTIF_FREC_MINS         3
#define NUM_MAX_INTENTOS        4

/**********************************************************************************************/
// VARIABLES GLOBALES
/**********************************************************************************************/
static volatile bool pastillaDetectada;

// Constructor
// Inicializar objetos
Dispensador::Dispensador() :
    brazo(PINS_MOTOR_BRAZO, PIN_MOTOR_NEUM, PIN_VALVULA, PIN_REED_BRAZO),
    bandeja(PINS_MOTOR_BAND, PIN_REED_BAND),
    hc05(/*RX1, TX1*/),
    teclado(PINS_TECLADO),
    reloj(PINS_RTC),
    pantalla(CLK_PIN, DATA_PIN, CS_PIN, RESET_PIN)
{
    pastADispensar[0] = false;
    pastADispensar[1] = false;
    pastADispensar[2] = false;
    pastADispensar[3] = false;

    isFinishedDispensing = false;
    isRetirado = false;
    pastillaDetectada = false;
    playOnce = true;
    missedCount = 0;
}

// Destructor
Dispensador::~Dispensador()
{
}

// -------------------------- Implementación --------------------------
void Dispensador::setup()
{
#ifdef DEBUG
    // Init serial port
    Serial.begin(57600);
    // Wait until serial port is ready
    while (!Serial);
#endif

    DEBUG_PRINTLN("--------------------------------");
    DEBUG_PRINTLN("Inicializando...");
    // Sensor para detectar pastillas dispensadas
    pinMode(PIN_SENSOR_PASTILLA, INPUT);
    // Vincular una interrupcion de cambio de flanco negativo al pin
    attachInterrupt(digitalPinToInterrupt(PIN_SENSOR_PASTILLA), Dispensador::pastillaDetectadaISR, FALLING);
    pinMode(PIN_REED_VASO, INPUT_PULLUP);
    pinMode(PIN_LED_VASO, OUTPUT);
    DEBUG_PRINTLN("- Pines configurados!");
    pantalla.setup();
    DEBUG_PRINTLN("- Pantalla inicializada!");
    brazo.retornarPosInicial();
    DEBUG_PRINTLN("- Brazo en posicion inicial!");
    bandeja.retornarPosInicial();
    DEBUG_PRINTLN("- Bandeja en posición inicial!");
    DEBUG_PRINTLN("- Informacion de las pastillas guardadas:")
    int eeAddress = 100;
    for (int i = 0; i < 4; i++)
    {
        EEPROM.get(eeAddress*(i+1), pastilla[i]);
        pastilla[i].getPillInfo();
    }
    DEBUG_PRINTLN("- Pastillas recuperadas de la EEPROM!");
    DEBUG_PRINTLN("Dispensador inicializado!");
    reloj.updateTime();
    DEBUG_PRINT("Hora: ");
    DEBUG_PRINT(reloj.hours); DEBUG_PRINT(":"); DEBUG_PRINT(reloj.minutes); DEBUG_PRINT(":"); DEBUG_PRINTLN(reloj.seconds); 
    DEBUG_PRINTLN("--------------------------------\n");
}

void Dispensador::loop()
{   
    reloj.updateTime();

    char* com;
    pantalla.botonPantalla = teclado.getTecla();
    pantalla.loop();

    pantalla.setDate(reloj.dayofmonth, reloj.month, reloj.year);
    pantalla.setTime(reloj.hours, reloj.minutes, reloj.seconds);
    pantalla.setRemainingPills(pastilla[0].pastillasRestantes, pastilla[1].pastillasRestantes, pastilla[2].pastillasRestantes, pastilla[3].pastillasRestantes);

    verificarHoraPastilla();

    com = hc05.getCommand();
    if(strncmp(com, "", 1) != 0)
    {
        DEBUG_PRINT("\n[BT]: ");
        DEBUG_PRINTLN(com);

        executeCommand(com);
    }

    com = pantalla.getCommand();
    if(strncmp(com, "", 1) != 0)
    {
        DEBUG_PRINT("\n[PANT]: ");
        DEBUG_PRINTLN(com);

        executeCommand(com);
    }
}

// -------------------------- Implementación --------------------------
void Dispensador::dispensarPastilla(uint8_t recipiente)
{
    bandeja.seleccionarPastilla(recipiente);
    brazo.succionPastilla();
    bandeja.retornarPosInicial();
    brazo.soltar();

    delay(1000); // Tiempo para que la pastilla caiga al vaso
}

void Dispensador::dispensarPastilla(uint8_t recipiente, uint8_t cantidad)
{
    for (int i = 0; i < cantidad; i++)
    {
        dispensarPastilla(recipiente);
    }
}

/*
    Pasar direccion de memoria de pastilla para conservar estado
*/
void Dispensador::dispensarPastilla(Pastilla &pastilla)
{
    isFinishedDispensing = false;
    pastillaDetectada = false;

    if (pastilla.getRecipiente() == 0)
    {
        DEBUG_PRINTLN("*Error: No existe la pastilla!");
        return;
    }

    if (pastilla.verificarCantidad())
    {
        DEBUG_PRINT("Dispensando ");
        DEBUG_PRINT(pastilla.getDosis());
        DEBUG_PRINT(" pastillas del recipiente ");
        DEBUG_PRINT(pastilla.getRecipiente());
        DEBUG_PRINT(" (");
        DEBUG_PRINT(pastilla.pastillasRestantes);
        DEBUG_PRINT(" restantes) ");

        // Seguir intentando dispensar pastilla hasta que se detecte que cayó 
        for (int intento = 0; intento < NUM_MAX_INTENTOS; intento++)
        {
            dispensarPastilla(pastilla.getRecipiente(), pastilla.getDosis());

            if(pastillaDetectada == true)
            {
                pastilla.pastillasRestantes -= pastilla.getDosis();
                break;
            }
        }
        
        if (pastillaDetectada == false)
        {
            DEBUG_PRINTLN("*Error: Nunca se detectó caer la pastilla!");
            return;
        }
        
        isFinishedDispensing = true;
        isRetirado = true;
        playOnce = true;

        int eeAddress = 100 * pastilla.getRecipiente();
        EEPROM.put(eeAddress, pastilla);
    }
    else
    {
        DEBUG_PRINTLN("Ya no quedan suficientes pastillas!");
        DEBUG_PRINT("(Quedan: ");
        DEBUG_PRINT(pastilla.pastillasRestantes);
        DEBUG_PRINT(", se necesitan: ");
        DEBUG_PRINT(pastilla.getDosis());
        DEBUG_PRINTLN(")");
    }
}

/*
    Pasar direccion de memoria de pastilla para conservar estado
*/
void Dispensador::eliminarPastilla(Pastilla &pastilla)
{   
    if(pastilla.getRecipiente() == 0)
    {
        DEBUG_PRINTLN("*Error: Pastilla ya estaba sin configurar!");
        return;
    }

    int eeAddress = 100 * pastilla.getRecipiente();
    pastilla.eliminar();
    EEPROM.put(eeAddress, pastilla);

    DEBUG_PRINTLN("Pastilla eliminada!");
}

void Dispensador::eliminarPastilla(uint8_t recipiente)
{
    eliminarPastilla(pastilla[recipiente]);
}

/*
    Pasar direccion de memoria de pastilla para conservar estado
*/
void Dispensador::agregarPastillas(Pastilla &pastilla, uint8_t cantidad)
{
    if(pastilla.getRecipiente() == 0)
    {
        DEBUG_PRINTLN("*Error: Pastilla no configurada!");
        return;
    }
    // agregarPastillas(pastilla.getRecipiente(), cantidad);
    pastilla.pastillasRestantes += cantidad;

    int eeAddress = 100 * pastilla.getRecipiente();
    EEPROM.put(eeAddress, pastilla);

    DEBUG_PRINT("Agregadas ");
    DEBUG_PRINT(cantidad);
    DEBUG_PRINT(" pastillas del recipiente ");
    DEBUG_PRINT(pastilla.getRecipiente());
    DEBUG_PRINT(" (");
    DEBUG_PRINT(pastilla.pastillasRestantes);
    DEBUG_PRINTLN(" en total)");
}

void Dispensador::agregarPastillas(uint8_t recipiente, uint8_t cantidad)
{
}

// Interrupt Service Routine (ISR)
void Dispensador::pastillaDetectadaISR()
{
    pastillaDetectada = true;
    DEBUG_PRINT("*");
}

/*
    Ejecuta el comando (char array) recibido
*/
void Dispensador::executeCommand(char* command)
{
    if(strncmp(command, "", 1) != 0)
    {   
        // Verifica el opcode para ejecutar la función correspondiente
        // Configura la pastilla
        if(strncmp(command, "SET", 3) == 0)
        {
            char opCode[4];
            int recipiente;     // Recipiente en la bandeja
            char nombre[20];    // Nombre de la pastilla
            int dosis;          // Numero de pastillas a dispensar
            int caducidad[3];   // dd, mm, aaaa
            int frecuencia;     // Cada cuánto tiempo se tiene que dispensar
            int primerDosis[2]; // hh, mm
            int duracionTratamiento;       
            int duracionTiempo; // Días/Semanas/Meses/Años
            int diario;         // Opcion para marcar si es diario o no
            int dias[7];        // Días a dispensar

            sscanf(command, "%s %i %s %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i", 
                opCode, &recipiente, nombre, 
                &dosis, 
                &caducidad[0], &caducidad[1], &caducidad[2],
                &frecuencia,
                &primerDosis[0], &primerDosis[1],
                &duracionTratamiento, &duracionTiempo,
                &diario, &dias[0], &dias[1], &dias[2], &dias[3], &dias[4], &dias[5], &dias[6]
                );

            // Map int to bool
            bool diarioAux = !!diario;
            bool diasAux[7];
            for (int i = 0; i < 7; i++)
            {
                diasAux[i] = !!dias[i];
            }

            pastilla[recipiente-1].setup(/*nombre,*/ dosis, recipiente, frecuencia, caducidad, primerDosis, duracionTratamiento, duracionTiempo, diarioAux, diasAux);

            int eeAddress = 100 * recipiente;
            EEPROM.put(eeAddress, pastilla[recipiente-1]);

            pastilla[recipiente-1].getPillInfo();
        }
        // Agrega pastillas a una pastilla ya configurada previamente
        else if (strncmp(command, "ADD", 3) == 0)
        {
            int recipiente, cantidad;
            char opCode[4];
            sscanf(command, "%s %i %i", opCode, &recipiente, &cantidad);

            agregarPastillas(pastilla[recipiente-1], cantidad);
        }
        // Elimina una pastilla configurada
        else if (strncmp(command, "DEL", 3) == 0)
        {
            int recipiente;
            char opCode[4];
            sscanf(command, "%s %i", opCode, &recipiente);

            eliminarPastilla(pastilla[recipiente-1]);
        }
        // Intenta dispensar una pastilla configurada
        else if (strncmp(command, "DIS", 3) == 0)
        {

            int recipiente;
            char opCode[4];
            sscanf(command, "%s %i", opCode, &recipiente);
            
            dispensarPastilla(pastilla[recipiente-1]);
        }
        // Prueba para medir la corriente del dispensador
        else if (strncmp(command, "TST", 3) == 0)
        {
            // testCorriente();
            // DEBUG_PRINTLN("Test para medir corriente iniciado!");
            // playMelody(1);
        }
        // Configura pastilla mediante el dispensador
        else if (strncmp(command, "PST", 3) == 0)
        {
            int recipiente;
            char opCode[4];
  
            sscanf(command, "%s %i", opCode, &recipiente);

            // Asigna pastilla auxiliar usando copy constructor
            pastilla[recipiente-1] = pantalla.getPastilla();

            int eeAddress = 100 * recipiente;
            EEPROM.put(eeAddress, pastilla[recipiente-1]);

            pastilla[recipiente-1].getPillInfo();
        }
        // Configura el RTC mediante la hora y fecha del teléfono
        else if (strncmp(command, "TIM", 3) == 0)
        {
            char opCode[4];
            int date[3]; // dd/MM/yyyy
            int time[3]; // hh:mm:ss

            sscanf(command, "%s %i %i %i %i %i %i", opCode, &date[0], &date[1], &date[2], &time[0], &time[1], &time[2]);
            
            reloj.setDS1302Time(time[2], time[1], time[0], 1, date[0], date[1], date[2]);
            DEBUG_PRINTLN("Hora configurada!");
            DEBUG_PRINT("Hora: ");
            DEBUG_PRINT(reloj.hours); DEBUG_PRINT(":"); DEBUG_PRINT(reloj.minutes); DEBUG_PRINT(":"); DEBUG_PRINTLN(reloj.seconds); 
        }
    }

    free(command);
}

void Dispensador::testCorriente()
{
    brazo.testCorriente();
    bandeja.testCorriente();
}

void Dispensador::playMelody(int opcion)
{

    // if(opcion == 1)
    // {
        int melody[8] = {NOTE_C5, NOTE_E5, NOTE_C6, NOTE_B5, NOTE_G5, 0, 0, 0};
        int noteDurations[8] = {4, 4, 4, 2, 2, 4, 4, 4};
    // }
    // else
    // {
    //     int melody[8] = {NOTE_C5, NOTE_E5, NOTE_C6, NOTE_B5, NOTE_G5, 0, 0, 0};
    //     int noteDurations[8] = {4, 4, 4, 2, 2, 4, 4, 4};
    // }

    for (int thisNote = 0; thisNote < 8; thisNote++) {
        // to calculate the note duration, take one second divided by the note type.
        // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 1000 / noteDurations[thisNote];
        tone(PIN_BUZZER, melody[thisNote], noteDuration);

        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 1.20;
        delay(pauseBetweenNotes);

        // stop the tone playing:
        noTone(PIN_BUZZER);
    }
}

void Dispensador::ledNotification()
{
    digitalWrite(PIN_LED_VASO, HIGH);
}

void Dispensador::verificarHoraPastilla()
{
    // Verificar para todas las pastillas
    for (int i = 0; i < 4; i++)
    {
        if(pastilla[i].getRecipiente() != 0)
        {
            uint8_t frecuenciaAux   = pastilla[i].getFrecuencia();
            int *primerDosisAux     = pastilla[i].getPrimerDosis();

            // Verifica que haya pasado las horas de la frecuencia desde la primer toma
            // TODO: Cambiar a horas y minutos
            // int temp = (reloj.hours - primerDosisAux[0]) % frecuenciaAux;
            // if(temp == 0 && (reloj.minutes == primerDosisAux[1]))

            int temp = (reloj.minutes - primerDosisAux[1]) % frecuenciaAux;
            if(temp == 0 && (reloj.seconds == 0))
            {
                pastADispensar[i] = true;
            }
        }
    }
    
    dispensarPastillas();
}

void Dispensador::dispensarPastillas()
{
    for (int i = 0; i < 4; i++)
    {
        if (pastADispensar[i] == true)
        {
            DEBUG_PRINT("\n[TIME ");
            DEBUG_PRINT(reloj.hours); DEBUG_PRINT(":"); DEBUG_PRINT(reloj.minutes); DEBUG_PRINT(":"); DEBUG_PRINT(reloj.seconds); 
            DEBUG_PRINT("]: ");
            DEBUG_PRINT("Es hora de tomar la pastilla "); DEBUG_PRINTLN(i+1);

            dispensarPastilla(pastilla[i]);
            delay(1000);
            pastADispensar[i] = false;
        } 
    }

    verificarVaso();
}

/**
 * Funcion que se encarga de verificar si el vaso con las pastillas ha sido retirado
 */
void Dispensador::verificarVaso()
{
    int minutesDispensado;

    if(isRetirado)
    {
        if(playOnce)
        {
            minutesDispensado = reloj.minutes;
            pantalla.dosisLista(reloj.hours, reloj.minutes);
            ledNotification();
            playMelody(1);
            playOnce = false;
        }

        // HIGH = Imán alejado, LOW = Imán pegado
        if(digitalRead(PIN_REED_VASO) == HIGH)
        {
            digitalWrite(PIN_LED_VASO, LOW);
            isRetirado = false;
            pantalla.pageIndex = 1;
            missedCount = 0;
            pantalla.dosisPerdida = false;
        }

        // Notificar cada cierto tiempo
        int temp = (reloj.minutes - minutesDispensado) % NOTIF_FREC_MINS;
        if(temp == 0 && (reloj.seconds == 0))
        {
            DEBUG_PRINT("\n[TIME ");
            DEBUG_PRINT(reloj.hours); DEBUG_PRINT(":"); DEBUG_PRINT(reloj.minutes); DEBUG_PRINT(":"); DEBUG_PRINT(reloj.seconds);
            DEBUG_PRINT("]: ");
            DEBUG_PRINTLN("Pastillas no retiradas!!");

            missedCount++;
            playMelody(1);
        }

        if(missedCount == 4)
        {
            pantalla.dosisPerdida = true;
        }
    }
}