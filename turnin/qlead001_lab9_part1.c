/*	Author: Quinn Leader qlead001@ucr.edu
 *  Partner(s) Name: NA
 *	Lab Section: 026
 *	Assignment: Lab 9  Exercise 1
 *	Exercise Description: Play three different tones
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void set_PWM(double frequency) {
    static double current_frequency;
    if (frequency != current_frequency) {
        if (!frequency) TCCR3B &= 0x08;
        else TCCR3B |= 0x03;
        if (frequency < 0.954) OCR3A = 0xFFFF;
        else if (frequency > 31250) OCR3A = 0x0000;
        else OCR3A = (short)(8000000 / (128 * frequency)) - 1;
        TCNT3 = 0;
        current_frequency = frequency;
    }
}

void PWM_on() {
    TCCR3A = (1 << COM3A0);
    TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
    set_PWM(0);
}

void PWM_off() {
    TCCR3A = 0x00;
    TCCR3B = 0x00;
}

void Tick() {
    unsigned char input = (~PINA)&0x7;
    if (input==0x1) set_PWM(261.63); // C4 = 261.63 Hz
    else if (input==0x2) set_PWM(293.66); // D4 = 293.66 Hz
    else if (input==0x4) set_PWM(329.63); // E4 = 329.63 Hz
    else set_PWM(0); // off
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
    PWM_on();
    while (1) {
        Tick();
    }
    return 1;
}
