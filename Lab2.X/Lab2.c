/*
 * Second lab for Digital 2 course
 * File:   lab_2.c
 * Author: Juan Diego Castillo Amaya
 * Student ID: 17074
 *
 * Created on Jan. 26, 2020
 */


// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
#define _XTAL_FREQ 4000000
char ESTADOS;
char ESTADOR;
char sum;
char n1;
char n2;
char change;
char COMP;
unsigned char SEGMENTO1[] = {0x3F,0x06,0x6D,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,};
unsigned char SEGMENTO2[] = {0x3F,0x06,0x6D,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,};

void LEDS (void);
void DISPLAY (void);
void setup(void);
void REBOTE(void);
void INTERCAMBIO(void);
void BITS(void);
void ALARMA(void);

void __interrupt() ISR(void){

    if(RBIF==1){
        RBIF= 0;
        LEDS(); 
        return;
    }
    if(ADIF ==1) {
        ADIF = 0;
        n1 = ADRESH;
        n2 = ADRESH;
        COMP = ADRESH;
        return;
    }       
    if (T0IF==1){
        REBOTE();
        T0IF=0;
        TMR0=2;
        PORTA =sum;
        return;
    }
}

void main(void) {
        setup();
        DISPLAY();
        INTERCAMBIO();    
}
void setup(void){
    //PUERTOS DE SALIDA Y ENTRADA
    TRISA = 0;
    TRISC = 0;
    TRISD = 0x0C;
    TRISB = 0xE0;
    ANSEL = 0;
    ANSELH = 0x20;
    OSCCON = 0x71;
    
    // CONFIGURACIÓN EL ADC
    ADCON0 = 0x75;
    ADCON1 = 0x00;
    
    // INTERRUPCIONES
    INTCON = 0xE8;
    PIE1bits.ADIE = 1;
    PIR1bits.ADIF = 0;
    IOCB = 0xC0;
    OPTION_REG = 0x80;
    TMR0 = 2;
    PORTC =0;
}

void LEDS (void){ 
    if(PORTBbits.RB7==1){
        ESTADOS =1;
        ESTADOR= 0;
        di();
    }
    if(PORTBbits.RB7==0 && ESTADOS == 1){
        sum=sum+1;
        ESTADOS=0;
        ei();
        return;
        }
    if(PORTBbits.RB6==1){
        ESTADOS=0;
        ESTADOR=1;
        di();
    }
    if(PORTBbits.RB6==0 && ESTADOR ==1){
        ESTADOR = 0;
        sum = sum-1;
        ei();
        return;
    }
}
void DISPLAY (void) {
    while(1){
        if(COMP > sum){
            PORTDbits.RD0 = 1;
        }
        else{
            PORTDbits.RD0 = 0;
        }
        __delay_ms(1);
        if (ADCON0bits.GO_DONE == 0){
        BITS();
        INTERCAMBIO();
        ADCON0bits.GO_DONE = 1;
        }
    }
}
void REBOTE(void){
    if(change == 1){
        change = 0;
        return;
    }
    else{
        change = 1;
        return;
    }
}
void INTERCAMBIO (void){
    if(change == 1){
        PORTC = SEGMENTO1 [n1];
        PORTDbits.RD3 = 1;
        PORTDbits.RD2 = 0;
        return;
    }
    else{
        PORTDbits.RD3 = 0;
        PORTDbits.RD2 = 1;
        PORTC = SEGMENTO2 [n2];
        return;
    }
}
void BITS (void){
    n1 = n1 & 0x0F;// se le hace un and a ni para eliminar los 4 bits mas significativos
    n2 = ((n2 & 0xF0)>>4);//se le hace un and a n2 para eliminar los 4 bits menos significativos a la vez 
                                //que se le hace un corrimiento
    return;
}

