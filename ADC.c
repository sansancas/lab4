#include "ADC.h"

//Funci�n para elegir en que canal iniciar el ADC
void adc_init(int channel){
    if(channel <= 13){ 
        ADCON0bits.CHS = channel;
    }
    else {
        ADCON0bits.CHS = 0b0000;
    }
    
    //Configuraci�n ADC
    ADCON1bits.VCFG0 = 0; //Voltaje referenciado de 0V
    ADCON1bits.VCFG1 = 0; //Voltaje referenciado de 5V
    ADCON0bits.ADCS = 0b10; // Fosc/32
     
    ADCON1bits.ADFM = 0; //Justificado a la izquierda
    ADCON0bits.ADON = 1;//Encendemos el m�dulo del ADC
     
    //Banderas e interrupciones
    PIR1bits.ADIF = 0; //Apagamos la bandera del ADC
    INTCONbits.PEIE = 1; //Habilitar interrupciones perif�ricas
    PIE1bits.ADIE = 1; //Habilitar interrupciones del ADC
    INTCONbits.GIE = 1; //Habilitar interrupciones globales
    ADCON0bits.GO = 1; //Activamos la lectura del ADC
    return;
}

int adc_read(){
    return ADRESH;
}

void adc_change_channel(int channel){
    if(channel <= 13){ 
        ADCON0bits.CHS = channel;
    }
    else {
        ADCON0bits.CHS = 0b0000;
    }
    return;
    
}

int adc_get_channel(){
    char canal = ADCON0bits.CHS; //Le asignamos a la variable canal el n�mero de canal del ADC
    return canal;
}

float map (int val, int minx, int maxx, int miny, int maxy){
    float vo;
    vo = (float)(val-minx)*(maxy-miny)/(maxx-minx)+ miny; 
    return vo;
}