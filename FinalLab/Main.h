
#define _XTAL_FREQ      8000000

#define ACK             1
#define NAK             0

#define ACCESS_CFG      0xAC
#define START_CONV      0xEE
#define READ_TEMP       0xAA
#define CONT_CONV       0x02

#define FAN_EN          PORTEbits.RE1
#define FAN_LED         PORTEbits.RE2
#define RTC_ALARM_NOT   PORTBbits.RB2
#define KEY_PRESSED	// set port definition KEY_PRESSED LED

#define Ch_Minus        0
#define Channel         1
#define Ch_Plus         2
#define Prev            3
#define Next            4
#define Play_Pause      5
#define Minus           6
#define Plus            7
#define EQ              8




