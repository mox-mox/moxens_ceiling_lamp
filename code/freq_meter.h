/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file ********************************************************************
*
* Atmel Corporation
*
* File              : Frequency_counter.h
* Compiler          : WinAVR GCC
* Revision          : $20100110
* Date              : $Date: 
* Updated by        : $Author: Marv Kausch
*
* Support mail      : avr@atmel.com
*
* Supported devices : The example is written for the ATTiny861, ATmega48_88_168
*
* AppNote           : AVR205 - Frequency Measurement Made Easy with Tiny and Mega AVR
*
* Description       : Header file for Frequency_counter
*
*
*
****************************************************************************/

/*! \page License
Copyright (c) 2009 Atmel Corporation. All rights reserved. 
 
Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer. 

2. Redistributions in binary form must reproduce the above copyright notice, 
this list of conditions and the following disclaimer in the documentation 
and/or other materials provided with the distribution.

3. The name of Atmel may not be used to endorse or promote products derived 
from this software without specific prior written permission.  

4. This software may only be redistributed and used in connection with an Atmel 
AVR product. 

THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED 
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE 
EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR 
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND 
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 */


// This is our standard header with additional sections that shall/can be used
// to document the code. For details on all the tags please see the Doxygen
// documentation installed with Doxygen. For a description on doxygen usage 
// see Comment_sourcecode_doxygen.DOC


// Example of mainpage:
// (placed either last in main.c or in a separate file documentation.h)

/* Doxygen documentation mainpage ********************************************/
/*! \mainpage
 * \section intro Introduction
 * This documents the software for application note AVR205.
 *
 * \section compinfo Compilation Info
 * This software was compiled with WinAVR GCC 20100110.
 * the source have to be modified.\n
 *
 * \section deviceinfo Device Info
 * ATMegaxxx8/Tinyx61, such as Mega48, Mega88, Mega168 and Tiny 261, Tiny461, Tiny861/
 *
 * \section codesize Code Size
 * 590 bytes of program flash memory; 6 bytes of SRAM
 *
 *
 * \section contactinfo Contact Info
 * For more info about Atmel AVR visit http://www.atmel.com/products/AVR/ \n
 * For application notes visit 
 * http://www.atmel.com/dyn/products/app_notes.asp?family_id=607 \n
 * Support mail: avr@atmel.com
 */


//! Prototypes

/*! \brief  AVR peripheral's initialization function
 *
 *  This function sends a command byte to the connected
 *  peripheral and waits for a returned status code.
 *
 *  \note  The peripheral must be initialized in advance.
 *
 */
void freq_cntr_init(void);


/*! \brief  When interrupts are used, this function is called to start the measurement process.\n
 *	\A Pin Change of state on T0 will cause the 16-bit frequency counter to start.\n
 *
 *  - This function is not used during Busy Wait (polling) \n
 *
 */
void freq_cntr_start_measurement(void);

/*! \brief  When interrupts are used, this function is called to start the measurement process.\n
 *	\A Pin Change of state on T0 will cause the 16-bit frequency counter to start.\n
 *
 *  - This function is not used during Busy Wait (polling) \n
 *  peripheral and waits for a returned status code.
 *
 */
unsigned int freq_cntr_get_result(void);

/*! \brief  Clear the result to makeready for the next frequency counting \n
 */
void freq_cntr_clear_result(void);




/*! \brief  When Busy Wait (USE_INTERRUPTS FALSE) is used, this function is called to start the measurement process.\n
 *	\A Pin Change of state on T0 will cause the 16-bit frequency counter to start.\n
 *
 *  - This function is not used during Busy Wait (polling) \n
 *
 */
unsigned int freq_cntr_get_frequency(void);


/*! \brief  Tests that the frequency count is not as large as 0xFFFF.\n
 *	\If the result, returned in the 16 bit unsigned variable freq_cntr_freq_divd_by_10 .\n
 *  \ reaches 0xFFFF, this is considered an Overflow condition so the result is returned as 0xFFFF.\n
 */
void ovf_test_and_return_result(void);

//! End of Prototypes

#ifndef F_CPU
/*! \brief Define default CPU frequency, if this is not already defined. */
#define F_CPU 8000000UL
#endif



#if defined(__AVR_ATtiny261__) | defined(__AVR_ATtiny461__) | defined(__AVR_ATtiny861__)  // this works with GCC

// The following uses Timer0 as a 16 bit counter, and TImer1 as the Gate timer, set to run for 100 msec
#define FREQ_CNTR_CTRLA			TCCR0A
#define FREQ_CNTR_CTRLB      	TCCR0B
#define GATE_CNTR_CTRLB         TCCR1B
#define TC_COUNTMODE			TCW0
#define PIN_CHG_MASK			PCMSK1
#define PIN_CHG_INT				PCINT14
#define GEN_INT_MASK			GIMSK
#define PIN_CHG_INT_ENAB		PCIE1
#define PIN						PINB
#define PIN_NO					PB6
#define	FREQ_CNTR_COUNT_LOW		TCNT0L
#define FREQ_CNTR_COUNT_HIGH	TCNT0H
#define FREQ_CNTR_INT_FLAG_REG	TIFR	// there are no seperate TIFR0 and TIFR1
#define GATE_CNTR_INT_FLAG_REG	TIFR
#define	FREQ_CNTR_OVF_FLAG		TOV0
#define GATE_CNTR_OVF_FLAG		TOV1
#define	GATE_CNTR_COUNT			TCNT1
#define	GATE_CNTR_COUNT_HI		TC1H	// Tinyx61 only
#define	GATE_CNTR_PRELOAD		255-(unsigned int)(F_CPU/(4096L*10L))//255
#define GATE_CNTR_INT_MASK_REG	TIMSK
#define GATE_CNTR_OVF_INT_ENAB	TOIE1
#define CLOCK_SEL_00			CS00
#define CLOCK_SEL_1				CS01
#define CLOCK_SEL_2				CS02
#define CLOCK_SEL_10			CS10
#define CLOCK_SEL_12			CS12	
#define CLOCK_SEL_13			CS13
#define GEN_INT_FLG_REG			GIFR
#define PIN_CHG_INT_FLAG		PCIF
#define GATE_CNTR_OVF_vect		TIMER1_OVF_vect
#define PIN_CHG_vect			PCINT_vect
#endif

#if defined(__AVR_ATmega48__) | defined(__AVR_ATmega88__) | defined(__AVR_ATmega168__)  

// The following uses Timer1 as a 16 bit counter, and Timer0 as the Gate timer, set to measure 100 msec
#define FREQ_CNTR_CTRLA			TCCR1A
#define FREQ_CNTR_CTRLB      	TCCR1B
#define GATE_CNTR_CTRLB         TCCR0B
#define TC_COUNTMODE			TCW0
#define PIN_CHG_MASK			PCMSK2
#define PIN_CHG_INT				PCINT21
#define GEN_INT_MASK			PCICR
#define PIN_CHG_INT_ENAB		PCIE2
#define PIN						PIND
#define PIN_NO					PD5
#define FREQ_CNTR_INT_FLAG_REG	TIFR1
#define	FREQ_CNTR_OVF_FLAG		TOV1
#define GATE_CNTR_INT_FLAG_REG	TIFR0
#define	GATE_CNTR_OVF_FLAG		TOV0
#define	FREQ_CNTR_COUNT_LOW		TCNT1L
#define	FREQ_CNTR_COUNT_HIGH	TCNT1H
#define	GATE_CNTR_COUNT			TCNT0
#define	GATE_CNTR_PRELOAD		255-195
#define GATE_CNTR_INT_MASK_REG	TIMSK0
#define GATE_CNTR_OVF_INT_ENAB	TOIE0
#define CLOCK_SEL_00			CS00
#define CLOCK_SEL_02			CS02
#define CLOCK_SEL_1				CS11
#define CLOCK_SEL_2				CS12
#define CLOCK_SEL_10			CS10
#define CLOCK_SEL_12			CS12	
#define CLOCK_SEL_13			CS13
#define GEN_INT_FLG_REG			GIFR
#define PIN_CHG_INT_FLAG		PCIF
#define GATE_CNTR_OVF_vect		TIMER0_OVF_vect
#define PIN_CHG_vect			PCINT2_vect
#endif
