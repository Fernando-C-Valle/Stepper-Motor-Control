#include <math.h>
#include <stdio.h>
#include <xc.h> 
#include <pic18f4550.h> //PIC18F4550 Library
#pragma  config FOSC = INTOSCIO_EC 

#define _XTAL_FREQ 8000000 //Define xtal frequency to 8MHz

//Function's prototypes
void oscillator(void);
void ADC(void);
unsigned int HIGH = 0; //Store the ADRESH register
unsigned int LOW = 0; //Store the ADRESL register

void main() {
    int i = 0;
    float diff = 0;
    int B = 0; 
    int current = 0;
    TRISD = 0; // Port D set as output
    TRISAbits.RA0 = 1; //RA0 pin defined as input
    oscillator();
    ADC();
    
        while(1){
            ADCON0bits.GO = 1; // GO/DONE conversion status bit, enabled setting 1
            while(ADCON0bits.DONE); 
                HIGH = ADRESH; //Takes the first 8 Most Significant Bits
                LOW = ADRESL; /* Because of the left justification set, the remainig 2 bits (since the ADC module has 
                                a 10 bit resolution), are stored here*/                     
                diff = (B - HIGH )*(427/0b11111111);//Store if the value is negative to know the motor's rotation.
                current = floor(A);//Floor the value of A so the sequences are smoother
                    
                //Depending on the potentiometer value (positive or negative) change the motor's rotation 
                if(current < 0){                    
                    for(i=current; i<0 ;i++){
                        //Enable the coils in the motor one at a time
                        LATD = 0b00001000; 
                        __delay_ms(3); //Set a 3ms delay for a smooth movement
                        LATD = 0b00000100;
                        __delay_ms(3);
                        LATD = 0b00000010;
                        __delay_ms(3);
                        LATD = 0b00000001;
                        __delay_ms(3);                                                                              
                    }
                }
                __delay_ms(10);
                if(i < 0){//Reset the iteraton variable
                    LATD = 0;                        
                }  
                            
                if(current > 0){                    
                    for(i=current; i>0 ;i--){
                        //Change the motor's direction by reversing the sequence
                        LATD = 0b00000001; 
                        __delay_ms(3);
                        LATD = 0b00000010;
                        __delay_ms(3);
                        LATD = 0b00000100;
                        __delay_ms(3);
                        LATD = 0b00001000;
                        __delay_ms(3);
                    }
                }                
                __delay_ms(10);
                if(i > 0){
                    LATD = 0;                        
                }                                            

                B = HIGH;//Update B value                    
        }
}

void oscillator (){
    OSCTUNE = 0; //Set oscillator frequency
    OSCCON = 0b11111110 ; 
    /*     
    *Bit 0-1, SCS1:SCS0: System Clock Select bits, 11 for internal clock. 
    *Bit 2, stable frequency, set as 1. 
    *Bit 3, priamary oscillator enabled, set as 1. 
    *Bits 4-6, 8MHz setting 111. 
    *Bit 7, sets sleep mode, set as 0. 
    */
}

void ADC(){
    ADCON1bits.VCFG = 0b00; // VDD as source and VSS as ground
    ADCON1bits.PCFG = 0b1110; // Select channel AN0 as analog channel
    ADCON0bits.CHS = 0b0000;  // 0000 = Channel 0 (AN0) is selected
    ADCON2bits.ADFM = 0; // 0 = Left justification
    ADCON2bits.ACQT = 0b010; // Acquisition time
    ADCON2bits.ADCS = 0b001; 
    ADCON0bits.ADON = 1; // ADC module enabled
}