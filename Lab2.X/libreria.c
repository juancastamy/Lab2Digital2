#include <xc.h>
#include "libreria.h"
void setup(void){
    //PUERTOS DE SALIDA Y ENTRADA
    TRISA = 0;
    TRISC = 0;
    TRISD = 0;
    TRISB = 0xE0;
    ANSEL = 0;
    ANSELH = 0x20;
    OSCCONbits.IRCF = 0b111;
    
    
    // CONFIGURACIÓN EL ADC
    ADCON0 = 0x35;
    ADCON1 = 0x00;
    
    // INTERRUPCIONES
    INTCON = 0xE8;
    PIE1bits.ADIE = 1;
    PIR1bits.ADIF = 0;
    IOCB = 0xC0;
    OPTION_REG = 0x80;
    TMR0 = 2;
    PORTC =0;
    n1 = 0;
    n2 = 0;
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
        n1 = ADRESH;
        n2 = ADRESH;
        COMP = ADRESH;
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
    if(change == 0){
        
        PORTC = SEGMENTO2 [n2];
        PORTDbits.RD3 = 0;
        PORTDbits.RD2 = 1;
        return;
    }
}
void BITS (void){
    n1 = n1 & 0x0F;// se le hace un and a ni para eliminar los 4 bits mas significativos
    n2 = ((n2 & 0xF0)>>4);//se le hace un and a n2 para eliminar los 4 bits menos significativos a la vez 
                                //que se le hace un corrimiento
    return;
}

