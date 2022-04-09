/* 
 * File:   main.c
 * Author: Jose Gonzalez
 *
 * Created on 3 domingo 2022
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
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits Write protection off)
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
        
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
//#define _XTAL_FREQ 4000000
#define _tmr0_value 251

void setup(void);
int convertidor(void); // convertidor de hexadecimal a decimal

// BITS PARA MOSTRAR LOS NUMEROS EN LOS DISPLAYS
char tabla [10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x67};

// VARIABLES
int banderas;
int unidades;
int decenas;
int centenas;
int residuo;
int numero;


void __interrupt() isr(void){
// INTERRUPCIONES DEL PUERTO B
    
    if(T0IF == 1){
        PORTD = 0X00;
        TMR0 = _tmr0_value;
        INTCONbits.T0IF = 0;
        
        if (banderas == 0){
            PORTDbits.RD0 = 1;
            PORTDbits.RD1 = 0;
            PORTDbits.RD2 = 0;
            
            PORTC = tabla[centenas];
            banderas = 1;
        }
        
        else if (banderas == 1){
            PORTDbits.RD0 = 0;
            PORTDbits.RD1 = 1;
            PORTDbits.RD2 = 0;
            
            PORTC = tabla[decenas];
            banderas = 2;
        }
        
        else if (banderas == 2){
            PORTDbits.RD0 = 0;
            PORTDbits.RD1 = 0;
            PORTDbits.RD2 = 1;
            
            PORTC = tabla[unidades];
            banderas = 0;
        }
        
    }
    
    if (RBIF == 1){
        if (RB0 == 1){
            PORTA++;
        }
        if (RB1 == 1){
            PORTA--;
        }
        INTCONbits.RBIF = 0;
    }
    return;
}

void main(void) {
    setup();
    while(1){
        numero = PORTA;
        convertidor();
    }
 return ;
}
    
void setup(void){
    ANSEL = 0X00;
    ANSELH = 0X00;
    
    TRISC = 0X00;
    TRISA = 0X00;
    TRISB = 0X03;
    TRISD = 0X00;
    
    PORTC = 0X00;
    PORTA = 0X00;
    PORTB = 0X00;
    PORTD = 0X00;
 
    // CONFIGURACION DE LAS INTERRUPCIONES
    INTCONbits.GIE = 1;
    INTCONbits.T0IE = 1;
    INTCONbits.RBIE = 1;
    INTCONbits.T0IF = 0;
    INTCONbits.RBIF = 0;
   
    // CONFIGURACION DEL OSCILADOR
    OSCCONbits.IRCF2 = 0;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS = 1;
    
    // CONFIGURACION DEL TIMER0
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS0 = 1;
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS2 = 1;
    OPTION_REGbits.T0CS = 0;
    TMR0 = 207;
    
    
    // CONFIGURACION DEL PUERTO B
    IOCBbits.IOCB0 = 1;
    IOCBbits.IOCB1 = 1;
    
    // SETEO DE VARIABLES
    banderas = 0x00;
    
}

int convertidor(void){
    centenas = numero/100;
    residuo = numero%100;
    decenas = residuo/10;
    unidades = residuo%10;

}