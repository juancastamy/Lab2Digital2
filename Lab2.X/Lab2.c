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
#define _XTAL_FREQ 4000000
char ESTADOS = 0;
char ESTADOR = 0;
char sum = 0;
void LEDS (void);
void DISPLAY (void);
void setup(void);

void main(void) {
    setup ();
    while(1){
        DISPLAY();
        LEDS();
    }
    }
void LEDS (void){
    while(1){
        if(PORTBbits.RB6 == 1){
            ESTADOS= 1;
        }
        if(PORTBbits.RB6 == 0 && ESTADOS ==1){
            sum = sum+1;
            PORTC = sum;
            ESTADOS = 0;
        }
        if(PORTBbits.RB7 == 1){
            ESTADOR= 1;
        }
        if(PORTBbits.RB7 == 0  && ESTADOR ==1){
            sum = sum-1;
            PORTC = sum;
            ESTADOR = 0;
        }
        return;
    }
}
void DISPLAY (void) {
    while (1){
        __delay_ms(1);
         if (ADCON0bits.GO_DONE == 0){
             ADCON0bits.GO_DONE = 1;
         }
         PORTA = ADRESH;
         return;
    }
}
void setup(void){
    //PUERTOS DE SALIDA Y ENTRADA
    TRISA = 0;
    TRISE = 1;
    TRISC = 0;
    //TRISD = 0b00001100;
    TRISB = 0b11100101;
    ANSEL = 0;
    ANSELH = 0b00100000;
    OSCCONbits.IRCF = 0b110;
    TRISEbits.TRISE3 = 1;
    TRISEbits.TRISE0 = 1;
    // CONFIGURACIÓN EL ADC
    ADCON0bits.ADCS = 0b01;//SE CONFIGURO LA FRECUENCIA DEL ADC
    ADCON0bits.CHS = 0b1101; //SE ECOGE EL PUERTO ENTRADA
    ADCON0bits.ADON = 1; //SE ENCIRENDE EL ADC
    ADCON1bits.ADFM = 0;
    ADCON1bits.VCFG1 = 0;
    ADCON1bits.VCFG0 = 0;
}