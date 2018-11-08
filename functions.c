#pragma config FOSC = INTOSCIO
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config MCLRE = OFF
#pragma config BOREN = OFF
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config CP = OFF

#include "functions.h"
#include <xc.h>

#define TRIS_S1 TRISAbits.TRISA1
#define TRIS_S2 TRISAbits.TRISA0
#define TRIS_S3 TRISAbits.TRISA4
#define TRIS_S4 TRISAbits.TRISA5
#define TRIS_S5 TRISBbits.TRISB2
#define TRIS_LED_R TRISBbits.TRISB6
#define TRIS_LED_A TRISBbits.TRISB7
#define TRIS_LED_V TRISAbits.TRISA6
#define TRIS_BTN_I TRISBbits.TRISB4
#define TRIS_BTN_D TRISBbits.TRISB5
#define TRIS_CNY TRISAbits.TRISA7
#define TRIS_CARRIER TRISBbits.TRISB3
#define TRIS_MOTI_1 TRISAbits.TRISA2
#define TRIS_MOTI_2 TRISAbits.TRISA3
#define TRIS_MOTD_1 TRISBbits.TRISB0
#define TRIS_MOTD_2 TRISBbits.TRISB1

#define PORT_S1 PORTAbits.RA1
#define PORT_S2 PORTAbits.RA0
#define PORT_S3 PORTAbits.RA4
#define PORT_S4 PORTAbits.RA5
#define PORT_S5 PORTBbits.RB2
#define PORT_LED_R PORTBbits.RB6
#define PORT_LED_A PORTBbits.RB7
#define PORT_LED_V PORTAbits.RA6
#define PORT_BTN_I PORTBbits.RB4
#define PORT_BTN_D PORTBbits.RB5
#define PORT_CNY PORTAbits.RA7
#define PORT_CARRIER PORTBbits.RB3
#define PORT_MOTI_1 PORTAbits.RA2
#define PORT_MOTI_2 PORTAbits.RA3
#define PORT_MOTD_1 PORTBbits.RB0
#define PORT_MOTD_2 PORTBbits.RB1

static unsigned long millisCounter = 0;

void interrupt ISR(void){
    static unsigned int decimicrosCounter = 0;
    if(!INTCONbits.T0IF) return;
    decimicrosCounter += 256;
    if(decimicrosCounter < 10000) return;
    millisCounter++;
    decimicrosCounter -= 10000;
    INTCONbits.T0IF = 0;
}

unsigned long millis(void){
    return millisCounter;
}

void setMotores(signed char izq, signed char der){
    PORT_MOTI_1 = (izq < 0);
    PORT_MOTI_2 = (izq > 0);
    PORT_MOTD_1 = (der < 0);
    PORT_MOTD_2 = (der > 0);
}

void initIO(void){
    TRIS_S1 = 1;
    TRIS_S2 = 1;
    TRIS_S3 = 1;
    TRIS_S4 = 1;
    TRIS_S5 = 1;
    TRIS_CNY = 1;
    TRIS_BTN_I = 1;
    TRIS_BTN_D = 1;
    
    TRIS_CARRIER = 0;
    TRIS_LED_R = 0;
    TRIS_LED_A = 0;
    TRIS_LED_V = 0;
    TRIS_MOTD_1 = 0;
    TRIS_MOTD_2 = 0;
    TRIS_MOTI_1 = 0;
    TRIS_MOTI_2 = 0;
}

void initPWM(void){
    PR2 = 104; // 104 para 38kHz
    CCPR1L = 0x3F; // 50% a 8 bits de resolucion... CCPR1L:CCP1CON<5:4> puede llegar a ser 210 en vez de esto
    CCP1CON = 0x0F; // pwm mode | LSBs 00
    T2CON = 0x05; // 0x05 para 38kHz
}

void initRest(void){
    CMCON = 0x07;
    VRCON = 0x00;
    INTCON = 0xE0;
    PCON = 0x0B;
    OPTION_REG = 0x87;
}

void led(char n, char v){
    if(n == R) PORT_LED_R = v != 0;
    if(n == A) PORT_LED_A = v != 0;
    if(n == V) PORT_LED_V = v != 0;
}

bit btn(char n){
    if(n == I) return !PORT_BTN_I;
    if(n == D) return !PORT_BTN_D;
    return 0;
}

bit ir(char n){
    if(n == 1) return !PORT_S1;
    if(n == 2) return !PORT_S2;
    if(n == 3) return !PORT_S3;
    if(n == 4) return !PORT_S4;
    if(n == 5) return !PORT_S5;
    return 0;
}

bit cny(void){
    return !PORT_CNY;
}