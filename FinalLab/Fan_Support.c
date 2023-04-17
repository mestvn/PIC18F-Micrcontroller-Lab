
#include <p18f4620.h>
#include "Main.h"
#include "Fan_Support.h"
#include "stdio.h"

extern char HEATER;
extern char duty_cycle;
extern char heater_set_temp;
extern signed int DS1621_tempF;

int get_duty_cycle(signed int temp, int set_temp)
{
    char diff = 0;          // got from the lab 
    if (temp >= set_temp) duty_cycle = 0;
    else if (temp < set_temp)
    {
        diff = set_temp - temp;
        if (diff > 50) duty_cycle = 100;
        else if (25<=diff && diff < 50) duty_cycle = diff * 2;
        else if (10<=diff && diff < 25) duty_cycle = diff*3/2;
        else if (0<= diff && diff < 10) duty_cycle = diff;
    }
    do_update_pwm(duty_cycle);
    return duty_cycle;
}

void Monitor_Heater()
{
    duty_cycle = get_duty_cycle(DS1621_tempF, heater_set_temp);
    do_update_pwm(duty_cycle);

    if (HEATER == 1) 
    {
        FAN_EN = 1;
    }
    else FAN_EN = 0;
}

void Toggle_Heater()            // Flips the fan function
{
    if(HEATER==0) Turn_On_Fan();
    else Turn_Off_Fan();
}

int get_RPM()
{
    int RPS = TMR3L / 2;        // read the count. Since there are 2 pulses per rev
                                // then RPS = count /2
    TMR3L = 0;                  // clear out the count
    return (RPS * 60);          // return RPM = 60 * RPS
}

void Turn_Off_Fan()             // Turn off enables and LED
{
    HEATER = 0;
    FAN_EN = 0;
    FAN_LED = 0;
}

void Turn_On_Fan()              // Turn on enables and LED
{
    HEATER = 1;
    do_update_pwm(duty_cycle);
    FAN_EN = 1;
    FAN_LED = 1;
}







