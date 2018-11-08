#include "functions.h"
#include <xc.h>

#define T_ATRAS 1000
#define T_ESPERA 4960

enum {
    MENU,
    ESPERA,
    ANALISIS,
    ATRAS,
    VUELTA,
    ATAQUE,
    IZQA,
    DERA,
    IZQ,
    DER
};

void loop(void){
    static unsigned int estado = MENU;
    static unsigned int direccion = DER;
    static unsigned int semidireccion = DER;
    static unsigned long m = 0;
    switch(estado){
        case MENU:
            if(btn(I)){
                led(R, 1);
                led(A, 0);
                led(V, 0);
                direccion = IZQ;
                estado = ESPERA;
                while(btn(I)) m = millis();
                break;
            }
            
            if(btn(D)){
                led(R, 0);
                led(A, 0);
                led(V, 1);
                direccion = DER;
                estado = ESPERA;
                while(btn(I)) m = millis();
                break;
            }
            
            setMotores(0, 0);
            led(R, ir(1) || ir(2));
            led(A, cny() || ir(5));
            led(V, ir(3) || ir(4));
            break;
            
        case ESPERA:
            if(millis() >= (m + T_ESPERA)){
                estado = ANALISIS;
                led(R, 0);
                led(A, 0);
                led(V, 1);
            }
            
            setMotores(0, 0);
            led(R, 1);
            led(A, millis() > (m + T_ESPERA / 2));
            led(V, 0);
            break;
            
        case ANALISIS:
            if(cny()){
                estado = ATRAS;
                m = millis();
                break;
            }
            
            if(ir(5)){
                if(direccion != VUELTA) semidireccion = direccion;
                direccion = VUELTA;
                estado = VUELTA;
                break;
            }
            
            if(ir(2) && ir(3)){
                estado = ATAQUE;
                break;
            }
            
            if(ir(2)){
                estado = IZQA;
                break;
            }
            
            if(ir(3)){
                estado = DERA;
                break;
            }
            
            if(ir(1)){
                estado = IZQ;
                break;
            }
            
            if(ir(4)){
                estado = DER;
                break;
            }
            
            estado = direccion;
            break;
            
        case ATRAS:
            if(millis() > (m + T_ATRAS)){
                estado = ANALISIS;
                break;
            }
            
            setMotores(-1, -1);
            break;
            
        case VUELTA:
            setMotores(-(semidireccion == IZQ), -(semidireccion == DER));
            break;
            
        case ATAQUE:
            setMotores(1, 1);
            break;
            
        case IZQA:
            setMotores(0, 1);
            direccion = IZQ;
            break;
            
        case DERA:
            setMotores(1, 0);
            direccion = DER;
            break;
            
        case IZQ:
            setMotores(-1, 1);
            direccion = IZQ;
            break;
            
        case DER:
            setMotores(1, -1);
            direccion = DER;
            break;
    }
}

void main(void){
    initIO();
    initPWM();
    initRest();
    while(1) loop();
}
