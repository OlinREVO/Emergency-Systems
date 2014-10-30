//#define F_CPU (1000000L)
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "api.h"

#define VOLT_PER_ANGLE 45/5//this is a placeholder.  Get the actual angles from Chassis?

int main (void) {
    //Enable ADC, set prescalar to 128 (slow down ADC clock)
    ADCSRA |= _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
    //Enable internal reference voltage
    ADCSRB &= _BV(AREFEN);
    //Set internal reference voltage as AVcc
    ADMUX |= _BV(REFS0);

    DDRC &=~_BV(PC4);
 
    uint8_t input = {0x08};
    uint8_t brake_angle;

    uint8_t msg[1];

    initCAN(NODE_brake);

    ADMUX &= ~(0x1F);
    ADMUX |= input;

    for(;;)
    {
        ADCSRA |=  _BV(ADSC);
        while(bit_is_set(ADCSRA, ADSC));
        //ADC is a macro to combine ADCL and ADCH
        uint16_t voltage = ADC;
        uint16_t angle = voltage*VOLT_PER_ANGLE;

        
    }

    return 1;
}

void handleCANmsg(uint8_t destID, uint8_t msgID, uint8_t* msg, uint8_t msgLen) 
{

}