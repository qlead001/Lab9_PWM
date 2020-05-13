/*	Author: Quinn Leader qlead001@ucr.edu
 *  Partner(s) Name: NA
 *	Lab Section: 026
 *	Assignment: Lab 9  Exercise 2
 *	Exercise Description: One button turns on and off, others change frequency
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

enum notes {
    C4,D4,E4,F4,
    G4,A4,B4,C5,
};
const double freq[] = {
    261.63,293.66,329.63,349.23,
    392.00,440.00,493.88,523.25,
};
const unsigned char len = 8;

unsigned char A0 = 0, A1 = 0, A2 = 0, on = 0, freq_index = 0;

void Tick() {
    unsigned char in0 = (~PINA)&0x1, in1 = (~PINA)&0x2, in2 = (~PINA)&0x4;
    if (in0 != A0) {
        A0 = in0;
        if (in0) on = on?0:1;
    }
    if (in1 != A1) {
        A1 = in1;
        if (in1 && freq_index < len-1) freq_index++;
    }
    if (in2 != A2) {
        A2 = in2;
        if (in2 && freq_index > 0) freq_index--;
    }
    if (on) set_PWM(freq[freq_index]);
    else set_PWM(0);
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
