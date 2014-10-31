//#define F_CPU (1000000L)
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "api.h"

// Values from voltage divider (dependent on circuit)
#define Vin 5 //Placeholder for input voltage to thermistor
#define R2 100 //Placeholder for second (known) resistor in voltage divider

//Values from thermistor, based on data sheet
//To use in Steinhart-Hart Equation
#define a 0.0014 //Placeholder, current values from wikipedia
#define b 0.000237 //Placeholder, current values from wikipedia
#define c 0.000000099 //Placeholder, current values from wikipedia

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
        uint16_t voltage = ADC; // reads in voltage into PC4
        uint16_t R_therm = ((R2 * Vin) / voltage) - R2;
	    uint16_t temp = 1 / (a + (b * log((double)R_therm)) + (c * ((log((double)R_therm))*(log((double)R_therm))*(log((double)R_therm)))));
    }

    return 1;
}

void handleCANmsg(uint8_t destID, uint8_t msgID, uint8_t* msg, uint8_t msgLen) 
{

}
