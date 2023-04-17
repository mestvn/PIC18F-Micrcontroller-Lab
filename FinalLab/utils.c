#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

#include <p18f4620.h>
#include "utils.h"

extern char found;
extern char Nec_code1;
extern short nec_ok;
extern char array1[21];
extern char duty_cycle;

char RGB[7] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07};

char check_for_button_input()
{       
    if (nec_ok == 1)
    {
            nec_ok = 0;


            INTCONbits.INT0IE = 1;                              // Enable external interrupt
            INTCON2bits.INTEDG0 = 0;                            // Edge programming for INT0 falling edge

            found = 0xff;
            for (int j=0; j< 21; j++)
            {
                if (Nec_code1 == array1[j]) 
                {
                    found = j;
                    j = 21;
                }
            }
            
            if (found == 0xff) 
            {
                printf ("Cannot find button \r\n");
                return (0);
            }
            else
            {
                return (1);
            }
    }
}

char bcd_2_dec (char bcd)
{
    int dec;
    dec = ((bcd>> 4) * 10) + (bcd & 0x0f);
    return dec;
}

int dec_2_bcd (char dec)
{
    int bcd;
    bcd = ((dec / 10) << 4) + (dec % 10);
    return bcd;
}

void Do_Beep()                                                  // normal beep
{
//    KEY_PRESSED = 1;
    Activate_Buzzer();
    Wait_One_Sec();
//    KEY_PRESSED = 0;
    Deactivate_Buzzer();
    do_update_pwm(duty_cycle);
}

void Do_Beep_Good()                                             // good button beep
{
    //    KEY_PRESSED = 1;
    Activate_Buzzer_2KHz();
    Wait_One_Sec();
//    KEY_PRESSED = 0;
    Deactivate_Buzzer();
    do_update_pwm(duty_cycle);
}

void Do_Beep_Bad()                                              // bad button beep
{
//    KEY_PRESSED = 1;
    Activate_Buzzer_500Hz();
    Wait_One_Sec();
//    KEY_PRESSED = 0;
    Deactivate_Buzzer();
    do_update_pwm(duty_cycle);
}

void Wait_One_Sec()
{
    for (int k=0;k<0x8000;k++);
}

void Activate_Buzzer()                                          // normal buzzer
{
    PR2 = 0b11111001 ;
    T2CON = 0b00000101 ;
    CCPR2L = 0b01001010 ;
    CCP2CON = 0b00111100 ;
}

void Activate_Buzzer_500Hz()                                    // bad buzzer
{
    PR2 = 0b11111001 ;
    T2CON = 0b00000111 ;
    CCPR2L = 0b01111100 ;
    CCP2CON = 0b00111100 ;
}

void Activate_Buzzer_2KHz()                                     // good buzzer
{
    PR2 = 0b11111001 ;
    T2CON = 0b00000101 ;
    CCPR2L = 0b01111100 ;
    CCP2CON = 0b00111100 ;
}

void Activate_Buzzer_4KHz()                                     // alarm buzzer
{
    PR2 = 0b01111100 ;
    T2CON = 0b00000101 ;
    CCPR2L = 0b00111110 ;
    CCP2CON = 0b00011100 ;
}

void Deactivate_Buzzer()
{
    CCP2CON = 0x0;
	PORTBbits.RB3 = 0;
}

void do_update_pwm(char duty_cycle) 
{ 
	float dc_f;
	int dc_I;
	PR2 = 0b00000100 ;                                          // Set the frequency for 25 Khz 
	T2CON = 0b00000111 ;                                        // As given in website
	dc_f = ( 4.0 * duty_cycle / 20.0) ;                         // calculate factor of duty cycle versus a 25 Khz signal
	dc_I = (int) dc_f;                                          // Truncate integer
	if (dc_I > duty_cycle) dc_I++;                              // Round up function
	CCP1CON = ((dc_I & 0x03) << 4) | 0b00001100;
	CCPR1L = (dc_I) >> 2;
}

void Set_DC_RGB(char duty_cycle)
{
    if(duty_cycle >= 70) PORTD = ((PORTD & 0xF0) | 0x07) & 0xFF;
    else if(duty_cycle < 9) PORTD = (PORTD & 0xF0) & 0xFF;
    else PORTD = ((PORTD & 0xF8) | duty_cycle/10) & 0xFF;
}

void Set_RPM_RGB(int rpm)
{
    if(rpm >= 3000) PORTD = ((PORTD & 0x1F) | 0xE0) & 0xFF;
    else if(rpm == 0) PORTD = (PORTD & 0x1F) & 0xFF;
    else PORTD = ((PORTD & 0x1F) | RGB[rpm/500]<<5) & 0xFF;
}

void Set_Alarm_RGB(char color)
{
    if (color >= 0) PORTA = (RGB[color]<<3);
    else if (color == 7) PORTA = (0x00<<3);
}

float read_volt()
{
    int num_step = get_full_ADC();                              // Obtain the steps of the ADC conversion
    float mv = num_step * 4.0;                                  // Convert the Analog to Digital conversion to a usable voltage measurement
    float volts = mv / 1000.0;
    return volts;
}

unsigned int get_full_ADC()
{
    int result;
    ADCON0bits.GO=1;                                            // Start Conversion
    while(ADCON0bits.DONE==1);                                  // Wait for conversion to be completed (DONE=0)
    result = (ADRESH * 0x100) + ADRESL;                         // Combine result of upper byte and lower byte into
    return result;                                              // return the most significant 8- bits of the result.
}

void Init_ADC()
{
    ADCON0 = 0x05;                                              // Channel 1, ADON bit = 0b00000101 
    ADCON1 = 0x0D;                          
    ADCON2 = 0xA9;                                              // Same as Lab 5 - right justify
}