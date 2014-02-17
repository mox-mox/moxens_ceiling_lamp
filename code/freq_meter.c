#include <avr/io.h>
##include <compat/ina90.h>	// where _NOP, _CLI and _SEI are defined
#include <avr/interrupt.h>
//#include "avr205_frequency_measure_interrupt.h"
#include "freq_meter.h"

#if defined(__AVR_ATmega48__) | defined(__AVR_ATmega88__) | defined(__AVR_ATmega168__)
unsigned int 	x48_cnt_to_four_ints;
#endif

unsigned int	freq_cntr_freq_divd_by_10;

/*! \brief  AVR peripheral's initialization function
 *
 *  This function sends a command byte to the connected
 *  peripheral and waits for a returned status code.
 *
 *  \note  The peripheral must be initialized in advance.
 *
 */
void freq_cntr_init(void) {

	// Initialize  16 bit FREQ_CNTR counts incoming positive edges on T0 input pin

#if defined(__AVR_ATtiny261__) | defined(__AVR_ATtiny461__) | defined(__AVR_ATtiny861__)
 
	FREQ_CNTR_CTRLA = (1<<TC_COUNTMODE); // Set Timer0 to 16 bit mode

#endif

#if defined(__AVR_ATmega48__) | defined(__AVR_ATmega88__) | defined(__AVR_ATmega168__)

	FREQ_CNTR_CTRLA = 0;	// Normal 16 bit mode OVF at 0xffff

#endif

	//Interrupts must be enabled for this code to work properly

}



//! GATE_CNTR is used as the time base for the 0.1 second sampling gate

/*! \brief  When interrupts are used, this function is called to start the measurement process.\n
 *	\A Pin Change of state on T0 will cause the 16-bit frequency counter to start.\n
 *
 *  - This function is not used during Busy Wait (polling) \n
 *
 */
void freq_cntr_start_measurement(void){
//! To detect a rising edge on PB6 with minimum latentcy, a Pin change interrupt is now enabled

#if defined(__AVR_ATtiny261__) | defined(__AVR_ATtiny461__) | defined(__AVR_ATtiny861__)

		PIN_CHG_MASK	= (1<<PIN_CHG_INT);
		GEN_INT_MASK	= (1<<PIN_CHG_INT_ENAB);
#endif

#if defined(__AVR_ATmega48__) | defined(__AVR_ATmega88__) | defined(__AVR_ATmega168__)

		PIN_CHG_MASK = (1<<PIN_CHG_INT);// for megax8;
		GEN_INT_MASK = (1<<PIN_CHG_INT_ENAB);
	
#endif


}

/*! \brief  Return the frequency divided by 10 to the main program \n
 *	\When freq_cntr_freq_divd_by_10 is not zero, it is sent to the main program by this function
 */
unsigned int	freq_cntr_get_result(void)
					{
						return freq_cntr_freq_divd_by_10;
					}


/*! \brief  Clear the result to makeready for the next frequency counting \n
 */
void	freq_cntr_clear_result(void)
					{
					 freq_cntr_freq_divd_by_10 = 0;
					}


ISR(PIN_CHG_vect) {		
		if ((PIN & (1<<PIN_NO)) !=0)
			{
				PIN_CHG_MASK	= (0<<PIN_CHG_INT);
				GEN_INT_MASK &= ~(1<<PIN_CHG_INT_ENAB);
				GATE_CNTR_INT_FLAG_REG	 |= (1<<GATE_CNTR_OVF_FLAG); // clear GATE_CNTR overflow flag if set 

// Enable FREQ_CNTR to count incoming signal on Tiny x61:PB6, Megax8: PD5

				FREQ_CNTR_COUNT_HIGH  = 0x00; // clear counter upper 8 bits
				FREQ_CNTR_COUNT_LOW  = 0x00; // clear counter lower 8  					
				FREQ_CNTR_CTRLB = (1<<CLOCK_SEL_2)|(1<<CLOCK_SEL_1);	// select FREQ_CNTR input clock

//The following counter pre-load was determined by the 0.1 second/512 usec period = 195.3 counts
//				TCNT1 = 255-195;// GATE_CNTR up counter loaded with this value to generate 0.1 sec delay
#if defined(__AVR_ATtiny261__) | defined(__AVR_ATtiny461__) | defined(__AVR_ATtiny861__)

			
				GATE_CNTR_COUNT = GATE_CNTR_PRELOAD;
				GATE_CNTR_COUNT_HI = 3;
				GATE_CNTR_INT_MASK_REG = (1<<GATE_CNTR_OVF_INT_ENAB);
#endif
#if defined(__AVR_ATmega48__) | defined(__AVR_ATmega88__) | defined(__AVR_ATmega168__)
				GATE_CNTR_COUNT = 0;
#endif

				GATE_CNTR_INT_MASK_REG = (1<<GATE_CNTR_OVF_INT_ENAB);

#if defined(__AVR_ATtiny261__) | defined(__AVR_ATtiny461__) | defined(__AVR_ATtiny861__)

				GATE_CNTR_CTRLB = (1<<CLOCK_SEL_13)|(1<<CLOCK_SEL_12)|(1<<CLOCK_SEL_10);//start GATE_CNTR
#endif

#if defined(__AVR_ATmega48__) | defined(__AVR_ATmega88__) | defined(__AVR_ATmega168__)

				GATE_CNTR_CTRLB = (1<<CLOCK_SEL_02)|(1<<CLOCK_SEL_00); // divide by 1024, the largest divisor possible in MEGAx8
				x48_cnt_to_four_ints = 0;  //Clear the x48 counter that counts 4 interrupts before closing gate
#endif

			}
		// else detected a negative-going pin change int, so ignore this
	}


////////////////////////////////

ISR(GATE_CNTR_OVF_vect) {

#if defined(__AVR_ATmega48__) | defined(__AVR_ATmega88__) | defined(__AVR_ATmega168__)
		x48_cnt_to_four_ints++;
		if(x48_cnt_to_four_ints == 3)GATE_CNTR_COUNT = 0xff-13;
		if(x48_cnt_to_four_ints >= 4)
		{
			FREQ_CNTR_CTRLB = 0;	// stop FREQ_CNTR
			GATE_CNTR_CTRLB = 0;

//	ovf_test_and_return_result(); //places result into freq_cntr_freq_divd_by_10
// Test TIFR for 16-bit overflow.  If overrange return  0xFFFF

			if ((FREQ_CNTR_INT_FLAG_REG & (1<<FREQ_CNTR_OVF_FLAG)) !=0) {
			// Test GATE_CNTR_OVF_FLAG for 16-bit overflow =  0xFFFF

				freq_cntr_freq_divd_by_10 = 0xFFFF; // This is to return a OVF condition
			}
		else {
			
			freq_cntr_freq_divd_by_10 = FREQ_CNTR_COUNT_LOW+(FREQ_CNTR_COUNT_HIGH<<8);

			}
		}
			
#endif




#if defined(__AVR_ATtiny261__) | defined(__AVR_ATtiny461__) | defined(__AVR_ATtiny861__)
{	
	  
		FREQ_CNTR_CTRLB = 0;
		GATE_CNTR_CTRLB = 0;		


//	ovf_test_and_return_result(); //places result into freq_cntr_freq_divd_by_10
// Test TIFR for 16-bit overflow.  If overrange return  0xFFFF
	
		if ((FREQ_CNTR_INT_FLAG_REG & (1<<FREQ_CNTR_OVF_FLAG)) !=0) {
		// Test GATE_CNTR_OVF_FLAG for 16-bit overflow =  0xFFFF
	
				freq_cntr_freq_divd_by_10 = 0xFFFF; // This is to return a OVF condition
			}
		else {
			freq_cntr_freq_divd_by_10 = FREQ_CNTR_COUNT_LOW+(FREQ_CNTR_COUNT_HIGH<<8);

			}
}

#endif

}
