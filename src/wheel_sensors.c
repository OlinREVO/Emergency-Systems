
#define F_CPU (1000000L)
#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <api.h>

#define TICKS_PER_ROTATION 45 // counted
#define WHEEL_DIAMETER .25 //get this from trike
#define PI 3.14159
#define DISTANCE_PER_TICK WHEEL_DIAMETER/2*PI/TICKS_PER_ROTATION
#define NUM_SAMPLES 10
#define DISTANCE_PER_CALCULATION DISTANCE_PER_TICK*NUM_SAMPLES

#define TIMESCALAR 1024.0
#define F_IO 1000000

volatile int head = 0;
volatile int dt[NUM_SAMPLES];

ISR(INT1_vect){
	dt[head] = TCNT1;
	head = (head+1)%NUM_SAMPLES;
    TCNT1 = 0;
}

void main(void){
	int i = 0;
	int time_sum = 0;
	uint8_t speed;

	for (i = 0; i<NUM_SAMPLES;i++){
		dt[i] = 0;
	}

	//Enable pin change interrupt 1(PCINT1) 
    PCICR |= _BV(PCIE0);

    // Set Timer/Counter1 on with prescaler at clk_io/1024 //for Hall latch
    TCCR1B |= (_BV(CS12) | _BV(CS10));

    initCAN(NODE_speedometer);

	while(1){
		time_sum = 0;
		for(i = 0; i<NUM_SAMPLES; i++){
			time_sum = time_sum + dt[i];
		}
		speed = (int)(DISTANCE_PER_CALCULATION/(time_sum/TIMESCALAR));

	}

	sendCANmsg(NODE_speedometer, MSG_speed,(uint8_t*)speed,1);
}

