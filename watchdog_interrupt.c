/****************************************************************************
Title:    Watchdog Timer Interrupt 
Author:   Elegantcircuits.com
File:     $Id: watchdog_interrupt.c
Software: AVR-GCC 3.3
Hardware: Atmega328P AVR 

Description:
    This example shows how to drive an LED periodically using the watchdog timer as a system clock

HW Description:

LED -> PB1

Reference:
http://electronics.stackexchange.com/questions/74840/use-avr-watchdog-like-normal-isr
*****************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/power.h>

// Function Prototypes
void init_devices(void);
void init_io(void);
void timer0_init(void);
void check_wdt(void);
void setup_wdt(void);

// Global Variables
volatile char tick_flag = 0;    /* if non-zero, a tick has elapsed */

int main(void){
    unsigned char ret;

    init_io();

    cli();
    check_wdt();
    setup_wdt();
    sei();                                      // Enables interrupts

    // Enable Sleep Mode for Power Down
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // Set Sleep Mode: Power Down
    sleep_enable();                     // Enable Sleep Mode  

    for(;;){                                                // Event Loop
        if(tick_flag){
            tick_flag = 0;
            sleep_disable();

            set_sleep_mode(SLEEP_MODE_PWR_DOWN);
            sleep_enable();
            sleep_mode();
        }
    }
}

void check_wdt(void){
    if(MCUSR & _BV(WDRF)){                  // If a reset was caused by the Watchdog Timer...
        MCUSR &= ~_BV(WDRF);                // Clear the WDT reset flag
        WDTCSR |= (_BV(WDCE) | _BV(WDE));   // Enable the WD Change Bit
        WDTCSR = 0x00;                      // Disable the WDT
    }
}

void setup_wdt(void){
// Set up Watch Dog Timer for Inactivity
    WDTCSR |= (_BV(WDCE) | _BV(WDE));                // Enable the WD Change Bit
                                                     // Enable WDT interrupt 
    //WDTCSR =   _BV(WDIE) |  _BV(WDP2) | _BV(WDP1);   // Set Timeout to ~1 seconds
    WDTCSR =   _BV(WDIE) | _BV(WDP2);   // Set Timeout to ~500 ms
}

//call this routine to initialize all peripherals
void init_devices(void){
    //stop errant interrupts until set up
    cli(); //disable all interrupts

    timer0_init();
    
    MCUCR = 0x00;
    EICRA = 0x00; //extended ext ints
    EIMSK = 0x00;
    
    TIMSK0 = 0x02; //timer 0 interrupt sources
    
    PRR = 0x00; //power controller
    sei(); //re-enable interrupts
    //all peripherals are now initialized
}

//TIMER0 initialize - prescale:1024
// WGM: CTC
// desired value: 10mSec
// actual value: 10.048mSec (-0.5%)
void timer0_init(void){
    TCCR0B = 0x00; //stop
    TCNT0 = 0x00; //set count
    TCCR0A = 0x02; //CTC mode
    OCR0A = 0xFF; 
    //OCR0A = 0x9C; 
    TCCR0B = 0x05; //start timer
}

void init_io(void){
    DDRB  = 0xff;                              // use all pins on port B for output 
    PORTB = 0x00;                              // (LED's low & off)
}

ISR(WDT_vect){
    // WDT has overflowed
    sleep_disable();

    PORTB ^= _BV(PB1);
    tick_flag = 1;

    sleep_enable();
}

//ISR(TIMER0_COMPA_vect){
// //TIMER0 has overflowed
//    PORTB ^= _BV(PB1);
//    tick_flag = 1;
//}

