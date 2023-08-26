/**
 * @file Pantalla.cpp
 * @author your name (you@domain.com)
 * @brief Archivo relacionado con las funciones para dibujar las páginas en la pantalla LCD
 * @version 0.5
 * @date 2022-05-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "Pantalla.h"
#include "DebugUtils.h"

// Constructor
Pantalla::Pantalla(int pinClk, int pinData, int pinCS, int pinReset) :
    u8g2(U8G2_R0, pinClk, pinData, pinCS, pinReset)
{
    this->pinClk = pinClk;
    this->pinData = pinData;
    this->pinCS = pinCS;
    this->pinReset = pinReset;

    botonPantalla = '\0';
    pageIndex = 1;

    horaAux  = false;
    fechaAux = false;
    dosisPerdida = false;

    resetVars();
}

// Destructor
Pantalla::~Pantalla()
{
}

// -------------------------- Implementación --------------------------
void Pantalla::setup()
{
    // pinMode(pinReset, OUTPUT);
    u8g2.begin();
    // digitalWrite(pinReset, LOW);
    // delay(100);
    // digitalWrite(pinReset, HIGH);

    // +----------------------------------------------------------------------------+
    // | Font: <u8g2> _ <font_5x8> _ <m> <f>                                        |
    // +----------------------------------------------------------------------------+
    // | <prefix> '_' <name> '_' <purpose> <char set>                               |
    // +----------------------------------------------------------------------------+
    // | <name>     font_5x8  - 6 pixel height font                                 |
    // | <purpose>  m         - All glyphs have common height and width (monospace) |
    // | <char set> f         - The font includes up to 256 glyphs                  |
    // +----------------------------------------------------------------------------+

    u8g2.setFont(u8g2_font_5x8_mr);
    u8g2.setFontMode(/* transparent = */ true);  
}

/**********************************************************************************************/
// UTILIDADES
/**********************************************************************************************/
void Pantalla::printHora()
{
    u8g2.setCursor(2, 19);
    u8g2.print(u8x8_u8toa(hh, 2));
    u8g2.print(":");
    u8g2.print(u8x8_u8toa(mm, 2));
    u8g2.print(":");
    u8g2.print(u8x8_u8toa(ss, 2));
}

void Pantalla::printFecha()
{
    u8g2.setCursor(75, 19);
    u8g2.print(u8x8_u8toa(dd, 2));
    u8g2.print("/");
    u8g2.print(u8x8_u8toa(MM, 2));
    u8g2.print("/");
    u8g2.print(u8x8_u16toa(aaaa, 4));
}

// Imprime el titulo y botones de navegacion
void Pantalla::setTitle(const char* title, int botones)
{   
    u8g2.drawFrame(0, 0, screenWidth, screenHeight);
    u8g2.drawBox(0,0,screenWidth, 11);
    u8g2.setDrawColor(2);
    u8g2.setCursor(3,9);
    u8g2.print(title);
    navButtons(botones);
    u8g2.setDrawColor(1);
}

// Imprime un subtitulo
void Pantalla::setSubtitle(const char* subtitle)
{   
    u8g2.drawStr(3, 19, subtitle);
}

void Pantalla::backButton()
{
    u8g2.drawButtonUTF8(81, 8, U8G2_BTN_HCENTER|U8G2_BTN_BW1, 24, 2, 0, "Back");
}

void Pantalla::okButton()
{
    u8g2.drawButtonUTF8(112, 8, U8G2_BTN_HCENTER|U8G2_BTN_BW1, 24, 2, 0, "Ok");
}

void Pantalla::navButtons(int opcion)
{
    switch (opcion)
    {
    case 1:
        backButton();
        break;

    case 2:
        okButton();
        break;

    case 3:
        backButton();
        okButton();
        break;
    
    default:
        break;
    }
}

void Pantalla::variableFlechas(int x, int y, char direccion)
{   
    int h = 10;
    int w = 36;

    u8g2.drawFrame(x, y, w, h-1);
    u8g2.drawVLine(x+(w/2), y, h-1);

    u8g2.setCursor(x+3, y+(h-2)-1);
    u8g2.print("-    +");

    if (direccion == 'h')
    {
        int yoffset  = y+2;
        int hoffset  = x+12;
        int hSpacing = hoffset+10;

        // Triangulo izquierda
        u8g2.drawVLine(hoffset+2, yoffset+0, 5);
        u8g2.drawVLine(hoffset+1, yoffset+1, 3);
        u8g2.drawVLine(hoffset+0, yoffset+2, 1);

        // Triangulo derecha
        u8g2.drawVLine(hSpacing+0, yoffset+0, 5);
        u8g2.drawVLine(hSpacing+1, yoffset+1, 3);
        u8g2.drawVLine(hSpacing+2, yoffset+2, 1);
    }
    else
    {
        int yoffset  = y+3;
        int hoffset  = x+11;
        int hSpacing = hoffset+10;

        // Triangulo arriba
        u8g2.drawHLine(hoffset+0, yoffset+0, 5);
        u8g2.drawHLine(hoffset+1, yoffset+1, 3);
        u8g2.drawHLine(hoffset+2, yoffset+2, 1);

        // Triangulo abajo
        u8g2.drawHLine(hSpacing+0, yoffset+2, 5);
        u8g2.drawHLine(hSpacing+1, yoffset+1, 3);
        u8g2.drawHLine(hSpacing+2, yoffset+0, 1);
    }
}

void Pantalla::mostrarVariable(int x, int y, const char nombre[5], int var, char direccion)
{
    u8g2.setCursor(x, y);
    u8g2.print(nombre);
    u8g2.print(" = ");
    u8g2.print(u8x8_u8toa(var, 2));

    variableFlechas(x+47, y-8, direccion);
}

void Pantalla::mostrarVariable(int x, int y, const char nombre[5], char direccion)
{
    u8g2.setCursor(x, y);
    u8g2.print(nombre);

    variableFlechas(x+47, y-8, direccion);
}

/*
    Opcion 1 = Tecla izquierda
    Opcion 2 = Tecla derecha
    Opcion 3 = Tecla abajo
    Opcion 4 = Tecla arriba
*/
void Pantalla::opcion(const char* opcionNombre, int numOpcion)
{
    int xoff = 3;
    int yoff = (numOpcion*10)+20;

    u8g2.setCursor(10, yoff);
    u8g2.print(opcionNombre);

    int xoff2 = xoff+1;
    int yoff2 = yoff-5;

    switch (numOpcion)
    {
    case 1:
        // Triangulo izquierda
        u8g2.drawVLine(xoff2+2, yoff2+0, 5);
        u8g2.drawVLine(xoff2+1, yoff2+1, 3);
        u8g2.drawVLine(xoff2+0, yoff2+2, 1);
        break;
    
    case 2:
        // Triangulo derecha
        u8g2.drawVLine(xoff2+0, yoff2+0, 5);
        u8g2.drawVLine(xoff2+1, yoff2+1, 3);
        u8g2.drawVLine(xoff2+2, yoff2+2, 1);
        break;
  
    case 3:
        // Triangulo arriba
        u8g2.drawHLine(xoff+0, yoff2+0, 5);
        u8g2.drawHLine(xoff+1, yoff2+1, 3);
        u8g2.drawHLine(xoff+2, yoff2+2, 1);
        break;
    
    case 4:
        // Triangulo abajo
        u8g2.drawHLine(xoff+0, yoff2+2, 5);
        u8g2.drawHLine(xoff+1, yoff2+1, 3);
        u8g2.drawHLine(xoff+2, yoff2+0, 1);
        break;
    
    default:
        break;
    }

}

void Pantalla::resetVars()
{
    dosis = 1;
    recipiente = 0;
    frec = 1;
    // cad = {1, 1, 2022};
    cad[0] = 1;
    cad[1] = 1;
    cad[2] = 2022;
    // primToma = {0, 0};
    primToma[0] = 0;
    primToma[1] = 0;
    durTrat = 1;
    durTiempo = 0;
    selectorDia = 1;
    // diasSemana = {false};
    diasSemana[0] = false;
    diasSemana[1] = false;
    diasSemana[2] = false;
    diasSemana[3] = false;
    diasSemana[4] = false;
    diasSemana[5] = false;
    diasSemana[6] = false;
    numSemanas = 0;
    numDias = 0;
    diario = false;

    cantidadAAgregar = 1;

    commandStr = "";
}

/**********************************************************************************************/
// PAGINAS A MOSTRAR
/**********************************************************************************************/
void Pantalla::p1menu()
{
    u8g2.firstPage();
    do
    {
        setTitle("MENU", NO_BTN);

        printFecha();
        printHora();

        opcion("Configurar", 1);
        opcion("Dispensar", 2);
    } while (u8g2.nextPage());

    menuBackend();
}

void Pantalla::menuBackend()
{
    resetVars();
    if(botonPantalla == 'l'){pageIndex++;}
    if(botonPantalla == 'r'){pageIndex = 10;}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Pantalla::p2selecPastilla()
{
    u8g2.firstPage();
    do
    {
        setTitle("PASTILLA", BACK_BTN);
        setSubtitle("Seleccionar pastilla");

        opcion("1", 1);
        opcion("2", 2);
        opcion("3", 3);
        opcion("4", 4);
    } while (u8g2.nextPage());

    selecPastillaBackend();
}

void Pantalla::selecPastillaBackend()
{
    if(botonPantalla == 'l'){recipiente = 1; pageIndex = 11;}
    if(botonPantalla == 'r'){recipiente = 2; pageIndex = 11;}
    if(botonPantalla == 'd'){recipiente = 3; pageIndex = 11;}
    if(botonPantalla == 'u'){recipiente = 4; pageIndex = 11;}

    if(botonPantalla == 'b'){pageIndex--;}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Pantalla::p3frecuencia()
{
    u8g2.firstPage();
    do
    {
        setTitle("FRECUENCIA", BACK_BTN);
        setSubtitle("Seleccionar");

        opcion("Diaria", 1);
        opcion("Dias x semana", 2);
    } while (u8g2.nextPage());

    frecuenciaBackend();
}

void Pantalla::frecuenciaBackend()
{
    if(botonPantalla == 'l')
    {
        diario = true;
        durTiempo = 1;
        pageIndex = pageIndex + 2;
    }
    
    if(botonPantalla == 'r')
    {
        diario = false;
        durTiempo = 2;
        pageIndex++;
    }

    if(botonPantalla == 'b'){pageIndex--;}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Pantalla::p4dias()
{
    u8g2.firstPage();
    do
    {
        int xoff = 60;

        setTitle("DIAS", BACK_OK_BTN);

        opcion("Borr" , 1);
        opcion("Selec", 2);
        opcion("Bajar", 3);
        opcion("Subir", 4);

        u8g2.drawStr(xoff, 16, "Lunes");
        u8g2.drawStr(xoff, 24, "Martes");
        u8g2.drawStr(xoff, 32, "Miercoles");
        u8g2.drawStr(xoff, 40, "Jueves");
        u8g2.drawStr(xoff, 48, "Viernes");
        u8g2.drawStr(xoff, 56, "Sabado");
        u8g2.drawStr(xoff, 64, "Domingo");

        // SELECTOR
        u8g2.setDrawColor(2);
        u8g2.drawBox(xoff-1, (selectorDia*8)+1, 46, 8);

        for (int i = 0; i < 7; i++)
        {
            if(diasSemana[i] == true)
                u8g2.drawStr(xoff-8, (i+2)*8, "*");
        }
    } while (u8g2.nextPage());

    diasBackend();
}

void Pantalla::diasBackend()
{
    if(botonPantalla == 'u' && selectorDia > 1){selectorDia--;}
    if(botonPantalla == 'd' && selectorDia < 7){selectorDia++;}

    if(botonPantalla == 'r'){diasSemana[selectorDia-1] = true;}
    if(botonPantalla == 'l'){diasSemana[selectorDia-1] = false;}

    if(botonPantalla == 'b'){pageIndex--;}
    if(botonPantalla == 'o'){pageIndex++;}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Pantalla::p5duracionTratamiento()
{
    u8g2.firstPage();
    do
    {
        setTitle("DURACION", BACK_OK_BTN);
        setSubtitle("Duracion y frec (hrs)");

        if(diario)
        {
            mostrarVariable(3, 45, "Dias", durTrat, 'h');
        }
        else
        {
            mostrarVariable(3, 45, "Sem ", durTrat, 'h');
        }

        mostrarVariable(3, 55, "Frec", frec, 'v');
    } while (u8g2.nextPage());

    duracionTratamientoBackend();
}

void Pantalla::duracionTratamientoBackend()
{
    if (botonPantalla == 'r' && durTrat < 99){durTrat++;}
    if (botonPantalla == 'l' && durTrat >  1){durTrat--;}

    if (botonPantalla == 'u' && frec < 24){frec++;}
    if (botonPantalla == 'd' && frec >  1){frec--;}

    if(botonPantalla == 'b'){pageIndex = pageIndex - 2;}
    if(botonPantalla == 'o'){pageIndex++;}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Pantalla::p6primerToma()
{
    u8g2.firstPage();
    do
    {
        setTitle("PRIMER TOMA", BACK_OK_BTN);
        setSubtitle("Seleccionar");

        u8g2.setCursor(36, 30);
        u8g2.print("Hora ");
        u8g2.print(u8x8_u8toa(primToma[0], 2));
        u8g2.print(":");
        u8g2.print(u8x8_u8toa(primToma[1], 2));
        mostrarVariable(3, 45, "Hora", 'h');
        mostrarVariable(3, 55, "Min ", 'v');
    } while (u8g2.nextPage());

    primerTomaBackend();
}

void Pantalla::primerTomaBackend()
{
    if(horaAux == false)
    {
        // Initialize time for user's convenience only once when entering the screen
        // (Perform only once to allow modifying the time)
        primToma[0] = hh;
        primToma[1] = mm;
        
        horaAux = true;
    }

    if (botonPantalla == 'l' && primToma[0] >  0){primToma[0]--;}
    if (botonPantalla == 'r' && primToma[0] < 23){primToma[0]++;}

    if (botonPantalla == 'd' && primToma[1] >  4){primToma[1] = primToma[1] - 5;}
    if (botonPantalla == 'u' && primToma[1] < 55){primToma[1] = primToma[1] + 5;}

    if(botonPantalla == 'b'){pageIndex--; horaAux = false;}
    if(botonPantalla == 'o'){pageIndex++; horaAux = false;}
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Pantalla::p7dosis()
{
    u8g2.firstPage();
    do
    {
        setTitle("DOSIS", BACK_OK_BTN);
        setSubtitle("Num de past a dispensar");
        mostrarVariable(3, 45, "Cant", dosis, 'h');
    } while (u8g2.nextPage());

    dosisBackend();
}

void Pantalla::dosisBackend()
{
    if (botonPantalla == 'l' && dosis >  1){dosis--;}
    if (botonPantalla == 'r' && dosis <  4){dosis++;}

    if(botonPantalla == 'b'){pageIndex--;}
    if(botonPantalla == 'o'){pageIndex++;}
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Pantalla::p8caducidad()
{
    u8g2.firstPage();
    do
    {
        setTitle("CADUCIDAD", BACK_OK_BTN);
        setSubtitle("Seleccionar");

        u8g2.setCursor(34, 30);
        u8g2.print("Fecha ");
        u8g2.print(u8x8_u8toa(cad[1], 2));
        u8g2.print("/");
        u8g2.print(u8x8_u16toa(cad[2], 4));

        mostrarVariable(3, 45, "Mes ", 'h');
        mostrarVariable(3, 55, "Ano ", 'v');
        u8g2.drawHLine(8, 49, 3); // Para la Ñ
    } while (u8g2.nextPage());

    caducidadBackend();
}

void Pantalla::caducidadBackend()
{
    if(fechaAux == false)
    {
        // Initialize date for user's convenience only once when entering the screen
        // (Perform only once to allow modifying the time)
        cad[1] = MM;
        cad[2] = aaaa;
        
        fechaAux = true;
    }

    if (botonPantalla == 'l' && cad[1] >   1){cad[1]--;}
    if (botonPantalla == 'r' && cad[1] <  12){cad[1]++;}

    if (botonPantalla == 'd' && cad[2] > 2022){cad[2]--;}
    if (botonPantalla == 'u' && cad[2] < 2099){cad[2]++;}

    if(botonPantalla == 'b'){pageIndex--; fechaAux = false;}
    if(botonPantalla == 'o'){pageIndex++; fechaAux = false;}
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Pantalla::p9resultado()
{
    u8g2.firstPage();
    do
    {
        setTitle("RESUMEN", BACK_OK_BTN);

        int vOffsetpx = 12;
        int vspacing  = 8;

        u8g2.setCursor(3, vOffsetpx+vspacing*1);
        u8g2.print("Pastilla #");
        u8g2.print(recipiente);

        u8g2.setCursor(3, vOffsetpx+vspacing*2);
        u8g2.print("Past a dispensar: ");
        u8g2.print(dosis);

        u8g2.setCursor(3, vOffsetpx+vspacing*3);
        u8g2.print("Cada: ");
        u8g2.print(frec);
        u8g2.print(" horas");

        u8g2.setCursor(3, vOffsetpx+vspacing*4);
        u8g2.print("Duracion: ");
        u8g2.print(durTrat);
        if(durTiempo == 1)
        {
            u8g2.print(" dias");
        }
        if(durTiempo == 2){
            u8g2.print(" sem");
        }

        u8g2.setCursor(3, vOffsetpx+vspacing*5);
        u8g2.print("Caducidad: ");
        u8g2.print(u8x8_u8toa(cad[1], 2));
        u8g2.print("/");
        u8g2.print(u8x8_u16toa(cad[2], 4));

    } while (u8g2.nextPage());

    resultadoBackend();
}

void Pantalla::resultadoBackend()
{
    if(botonPantalla == 'b'){pageIndex--;}
    if(botonPantalla == 'o')
    {
        pageIndex = 12; // Saltar a pantalla para agregar pastillas
        setPastilla();

        String temp = "PST ";
        commandStr = temp + recipiente;
    }
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Pantalla::p10dispensar()
{
    u8g2.firstPage();
    do
    {
        setTitle("DISPENSAR", BACK_BTN);
        setSubtitle("Seleccionar");

        for (int i = 0; i < 4; i++)
        {
            int yAux = 10;
            u8g2.setCursor(50, 30+(yAux*i)); 

            if(remainingPills[i] != 255) // -1
            {
                opcion("Past ", (i+1));
                u8g2.print((i+1));
                u8g2.print(" (");
                u8g2.print(remainingPills[i]);
                u8g2.print(" restantes)");
            }
        }

    } while (u8g2.nextPage());
    
    dispensarBackend();
}

void Pantalla::dispensarBackend()
{
    if (botonPantalla == 'l'){commandStr = "DIS 1";}
    if (botonPantalla == 'r'){commandStr = "DIS 2";}

    if (botonPantalla == 'd'){commandStr = "DIS 3";}
    if (botonPantalla == 'u'){commandStr = "DIS 4";}

    if(botonPantalla == 'b'){pageIndex = 1; /*commandStr = "";*/}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Pantalla::p11opciones()
{
    u8g2.firstPage();
    do
    {
        setTitle("OPCIONES", BACK_BTN);
        setSubtitle("Seleccionar");

        opcion("Configurar", 1);
        opcion("Agregar", 2);
        opcion("Eliminar", 3);
    } while (u8g2.nextPage());
    
    opcionesBackend();
}

void Pantalla::opcionesBackend()
{
    if (botonPantalla == 'l'){pageIndex = 3;}   // Configurar pastillas
    if (botonPantalla == 'r'){pageIndex = 12;}  // Agregar pastillas
    if (botonPantalla == 'd'){pageIndex = 13;}// Eliminar pastilla

    if(botonPantalla == 'b'){pageIndex = 2;}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Pantalla::p12agregar()
{
    u8g2.firstPage();
    do
    {
        setTitle("AGREGAR", BACK_OK_BTN);
        setSubtitle("Seleccionar cantidad");

        mostrarVariable(3, 45, "Cant", cantidadAAgregar, 'h');
    } while (u8g2.nextPage());
    
    agregarBackend();
}

void Pantalla::agregarBackend()
{
    if (botonPantalla == 'r' && cantidadAAgregar < 50){cantidadAAgregar++;}
    if (botonPantalla == 'l' && cantidadAAgregar >  1){cantidadAAgregar--;}

    if(botonPantalla == 'o')
    {
        String temp = "ADD ";
        commandStr = temp + recipiente + " " + cantidadAAgregar; 
        pageIndex = 1; 
    }

    if(botonPantalla == 'b'){pageIndex = 11;}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Pantalla::p13eliminar()
{
    u8g2.firstPage();
    do
    {
        setTitle("ELIMINAR", NO_BTN);
        setSubtitle("Seguro?");

        opcion("Si", 1);
        opcion("No", 2);
    } while (u8g2.nextPage());
    
    eliminarBackend();
}

void Pantalla::eliminarBackend()
{
    if (botonPantalla == 'l')
    {
        String temp = "DEL ";
        commandStr = temp + recipiente; 
        pageIndex = 1; 
    }
    if(botonPantalla == 'r'){pageIndex = 11;}

    if(botonPantalla == 'b'){pageIndex = 1;}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Pantalla::pAviso()
{
    u8g2.firstPage();
    do
    {
        setTitle("AVISO", OK_BTN);
 
        u8g2.setCursor(3, 20);
        if(!dosisPerdida)
        {
            u8g2.print("Pastillas dispensadas!");
        }
        else
        {
            u8g2.print("Dosis omitida!");
        }

        u8g2.setCursor(3, 40); 
        u8g2.print("Hora ");
        u8g2.print(u8x8_u8toa(hhAux, 2));
        u8g2.print(":");
        u8g2.print(u8x8_u8toa(mmAux, 2));
    } while (u8g2.nextPage());

    avisoBackend();
}

void Pantalla::avisoBackend()
{
    if(botonPantalla == 'o'){pageIndex = 1;}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Pantalla::loop()
{
    switch (pageIndex)
    {
    case 1:
        p1menu();
        break;
    
    case 2:
        p2selecPastilla();
        break;

    case 3:
        p3frecuencia();
        break;

    case 4:
        p4dias();
        break;

    case 5:
        p5duracionTratamiento();
        break;

    case 6:
        p6primerToma();
        break;

    case 7:
        p7dosis();
        break;

    case 8:
        p8caducidad();
        break;

    case 9:
        p9resultado();
        break;

    case 10:
        p10dispensar();
        break;

    case 11:
        p11opciones();
        break;

    case 12:
        p12agregar();
        break;
    
    case 13:
        p13eliminar();
        break;
    
    case 14:
        pAviso();
        break;
    
    default:
        break;
    }
}

void Pantalla::setPastilla()
{
    pastillaAux.setup(
        /*" ",*/
        dosis,
        recipiente, 
        frec,
        cad,
        primToma, 
        durTrat,
        durTiempo,
        diario,
        diasSemana
    );
}

Pastilla Pantalla::getPastilla()
{
    return pastillaAux;
}

char* Pantalla::getCommand()
{
    if(commandStr != "")
    {
        int tempSize = commandStr.length() + 1;
        int allocSize = tempSize*sizeof(char);
        char* temp = (char*) malloc(allocSize); // Allocate memory and cast it to char*
        strcpy(temp, commandStr.c_str());
        commandStr = "";   // Clear string so it doesn't accumulate characters

        return temp;
    }

    return "";
}

void Pantalla::setTime(int hh, int mm, int ss)
{
    this->hh = hh;
    this->mm = mm;
    this->ss = ss;
}

void Pantalla::setDate(int dd, int MM, int aaaa)
{
    this->dd = dd;
    this->MM = MM;
    this->aaaa = aaaa;
}

void Pantalla::setRemainingPills(int p1, int p2, int p3, int p4)
{
    remainingPills[0] = p1;
    remainingPills[1] = p2;
    remainingPills[2] = p3;
    remainingPills[3] = p4;
}

void Pantalla::dosisLista(int hh, int mm)
{
    hhAux = hh;
    mmAux = mm;
    pageIndex = 14;
}