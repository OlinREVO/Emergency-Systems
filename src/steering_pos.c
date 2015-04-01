#define TRANSITION 2^15-2^9 //if the voltage is within < 1 degree of center, assume it has changed quadrants
#define ADC_PER_DEGREE 5/2^15

void main(void){
	//left
	int quadrant = 0;
	int positions[10]={};
	int counter = 0;

	//slightly right of center
	int new_pos = TRANSISTION+1;

	//Enable ADC, set prescalar to 128 (slow down ADC clock)
    ADCSRA |= _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
    //Enable internal reference voltage
    ADCSRB &= _BV(AREFEN);
    //Set internal reference voltage as AVcc
    ADMUX |= _BV(REFS0);

    ADMUX &= ~(0x1F);
    ADMUX |= 0x06;

	while(1){
		ADCSRA |=  _BV(ADSC);
        while(bit_is_set(ADCSRA, ADSC));

        //ADC is a macro to combine ADCL and ADCH
        uint16_t pos_reading = ADC;

        if(pos >= TRANSITION){
        	quadrant = !quadrant;
        	new_pos = ADC_PER_DEGREE*90;
        }
        else{
	        if(quadrant){//wheel on right
	        	new_pos = pos_reading*ADC_PER_DEGREE+90*ADC_PER_DEGREE;
	        }
	        else{//wheel on left
	        	new_pos = pos_reading*ADC_PER_DEGREE;
	        }
	    }

	    positions[counter%10] = new_pos;
	    counter ++;
	}
}