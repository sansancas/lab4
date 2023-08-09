#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
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

#include <xc.h>
#include <stdlib.h>
#include <stdio.h>
#include "I2C.h"
#include "LCD.h"

#define _XTAL_FREQ 8000000
#define RS RE0
#define EN RE1
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

#define date 0x04
#define month 0x05
#define year 0x06
#define hour 0x02
#define min  0x01
#define sec  0x00

char s1_pot [10];
char dia_str [3];
char mes_str [3];
char ye_str [3];
char hora_str [3];
char min_str [3];
char sec_str [3];

uint8_t dia = 0x01, mes = 0x01, ye = 0x20, hora = 0x01, seg= 0x00, minutos=0x00;
uint8_t h,m,s,d,me,an;

void setup(void);

uint8_t bcd_to_decimal(uint8_t bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

void main(void)
{
    setup();
    Lcd_Init();
    while(1)
    {
        
        I2C_Master_Start();
        I2C_Master_Write(0x51);
        PORTB = I2C_Master_Read(0);
        int pot_s1 = PORTB;
        I2C_Master_Stop();
        __delay_ms(200);
        
        // Dia 
        I2C_Master_Start();          // Iniciar el I2C
        I2C_Master_Write(0xD0);      // Escribir la direccion del RTC para escritura
        I2C_Master_Write(date);      // Seleccionar la direccion para los dias
        I2C_Master_Write(dia);      // Día: 1
        I2C_Master_RepeatedStart();  // Reiniciar el I2C
        I2C_Master_Write(0xD1);      // Escribir la direccion del RTC para lectura
        d = I2C_Master_Read(0);
        I2C_Master_Stop();
        
        // Mes
        I2C_Master_Start();          // Iniciar el I2C
        I2C_Master_Write(0xD0);      // Escribir la direccion del RTC para escritura
        I2C_Master_Write(month);     // Seleccionar la direccion para los meses
        I2C_Master_Write(mes);      // Mes: 1
        I2C_Master_RepeatedStart();  // Reiniciar el I2C
        I2C_Master_Write(0xD1);      // Escribir la direccion del RTC para lectura
        me = I2C_Master_Read(0);
        I2C_Master_Stop();
        
        // Año
        I2C_Master_Start();          // Iniciar el I2C
        I2C_Master_Write(0xD0);      // Escribir la direccion del RTC para escritura
        I2C_Master_Write(year);      // Seleccionar la direccion para los años
        I2C_Master_Write(ye);        // Año: 20
        I2C_Master_RepeatedStart();  // Reiniciar el I2C
        I2C_Master_Write(0xD1);      // Escribir la direccion del RTC para lectura
        an = I2C_Master_Read(0);
        I2C_Master_Stop();
        
        // Hora
        I2C_Master_Start();          // Iniciar el I2C
        I2C_Master_Write(0xD0);      // Escribir la direccion del RTC para escritura
        I2C_Master_Write(hour);      // Seleccionar la direccion para los años
        I2C_Master_Write(hora);      // Hora 1
        I2C_Master_RepeatedStart();  // Reiniciar el I2C
        I2C_Master_Write(0xD1);      // Escribir la direccion del RTC para lectura
        h = I2C_Master_Read(0);
        I2C_Master_Stop();
        
        // Minutos
        I2C_Master_Start();          // Iniciar el I2C
        I2C_Master_Write(0xD0);      // Escribir la direccion del RTC para escritura
        I2C_Master_Write(min);      // Seleccionar la direccion para los años
        I2C_Master_RepeatedStart();  // Reiniciar el I2C
        I2C_Master_Write(0xD1);      // Escribir la direccion del RTC para lectura
        m = I2C_Master_Read(0);
        I2C_Master_Stop();
        
        // Segundos
        I2C_Master_Start();          // Iniciar el I2C
        I2C_Master_Write(0xD0);      // Escribir la direccion del RTC para escritura
        I2C_Master_Write(sec);      // Seleccionar los segundos
        I2C_Master_RepeatedStart();  // Reiniciar el I2C
        I2C_Master_Write(0xD1);      // Escribir la direccion del RTC para lectura
        s = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
        // Cambiar la fecha
        if (RA0 == 1)
        {
            dia++;
            if (dia > 31)
            {
                dia = 1;
            }
            
            I2C_Master_Start();
            uint8_t dia_decimal = bcd_to_decimal(dia); //Conversion de dias a decimal
            I2C_Master_Write(0xD0);
            I2C_Master_Write(date);
            I2C_Master_Write(dia_decimal);
            I2C_Master_RepeatedStart();  // Reiniciar el I2C
            I2C_Master_Write(0xD1);      // Escribir la direccion del RTC para lectura
            ye = I2C_Master_Read(0);
            I2C_Master_Stop();
             __delay_ms(200);
        }
        
        if (RA1 == 1)
        {
            mes++;
            if (mes > 12)
            {
                mes = 1;
            }
            
            I2C_Master_Start();
            uint8_t mes_decimal = bcd_to_decimal(mes); //Conversion de dias a decimal
            I2C_Master_Write(0xD0);
            I2C_Master_Write(month);
            I2C_Master_Write(mes_decimal);
            I2C_Master_Stop();
             __delay_ms(200);
        }
        if (RA2 == 1)
        {
            ye++;
            if (ye > 99)
            {
                ye = 0;
            }
            
            I2C_Master_Start();
            uint8_t decade = (ye >> 4);
            uint8_t year_of_decade = (ye & 0x0F);
            uint8_t ye_decimal = (decade * 10) + year_of_decade; //Conversion de dias a decimal
            I2C_Master_Write(0xD0);
            I2C_Master_Write(year);
            I2C_Master_Write(ye_decimal);
            I2C_Master_Stop();
             __delay_ms(200);
        }
        
        if (RA3 == 1)
        {
            hora++;
            if (hora > 24)
            {
                hora = 0;
            }
             __delay_ms(200);
        }
        
       //------------LCD----------------------------
       uint8_t dia_decimal = bcd_to_decimal(d); //Conversion de dias a decimal
       uint8_t mes_decimal = bcd_to_decimal(me);
       uint8_t decade = (an >> 4);
       uint8_t year_of_decade = (an & 0x0F);
       uint8_t ye_decimal = (decade * 10) + year_of_decade; //Conversion de dias a decimal
       uint8_t hora_decimal = bcd_to_decimal(h);
       uint8_t min_decimal = bcd_to_decimal(m);
       uint8_t seg_decimal = bcd_to_decimal(s);
       
       Lcd_Clear();                         // Iniciar la LCD
       Lcd_Set_Cursor(1,1);                 // Posicion del cursor
       Lcd_Write_String("Tmp:");            // Caracter a mostrar
       Lcd_Set_Cursor(1,5);                 // Posicionar el cursor 
       sprintf(s1_pot, "%d\r", pot_s1);     // Retransmitir el valor a char
       Lcd_Write_String(s1_pot);            // Escribir el valor del potenciometro
       Lcd_Set_Cursor(2,1);
       Lcd_Write_String("Fecha:");          // Caracter a mostrar de fecha
       Lcd_Set_Cursor(2,7);
       sprintf(dia_str, "%d",dia_decimal);  // Retransmitir el valor a char
       Lcd_Write_String(dia_str);           // Dias
       Lcd_Set_Cursor(2,9);                             
       sprintf(mes_str, "/%d",mes_decimal);  // Retransmitir el valor a char
       Lcd_Write_String(mes_str);           // Mes
       Lcd_Set_Cursor(2,12);
       sprintf(ye_str, "/%d",ye_decimal);
       Lcd_Write_String(ye_str);            // Años
       
       // Mostrar la hora en la LCD
       //-----Hora-------------------
       Lcd_Set_Cursor(1,9);
       Lcd_Write_String("T:");              
       Lcd_Set_Cursor(1,11);
       sprintf(hora_str, "%d:",hora_decimal);
       Lcd_Write_String(hora_str);
       //---------Minutos---------------
       Lcd_Set_Cursor(1,13);
       sprintf(min_str, "%d:",min_decimal);
       Lcd_Write_String(min_str);
       //-----------Segundos------------
       Lcd_Set_Cursor(1,15);
       sprintf(sec_str, "%d",seg_decimal);
       Lcd_Write_String(sec_str);
       __delay_ms(200);
        
    }
    return;
}
void setup(void)
{
    ANSEL = 0;
    ANSELH = 0;
    
    TRISA = 0;
    TRISB = 0;
    TRISD = 0;
    TRISE = 0b1100;
    PORTA = 0;
    PORTB = 0;
    PORTD = 0;
    PORTE = 0;
    
    OSCCONbits.IRCF = 0b111; //8 MHz
    OSCCONbits.SCS = 1;
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
}