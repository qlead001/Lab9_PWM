/*	Author: Quinn Leader qlead001@ucr.edu
 *  Partner(s) Name: NA
 *	Lab Section: 026
 *	Assignment: Lab 9  Exercise 3
 *	Exercise Description: Play melody
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "timer.h"
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

typedef enum notes {
    OFF,
    C4,D4,E4,F4,
    G4,A4,B4,C5,
} notes;
const double freq[] = {
    0,
    261.63,293.66,329.63,349.23,
    392.00,440.00,493.88,523.25,
};
const unsigned char len = 9;

unsigned char A0 = 0, on = 0, count = 0;

const notes melody[] = {B4,G4,B4,OFF,C4,OFF,C5,OFF,C4,D4,OFF,C5,B4};
const unsigned char periods[] = {10,3,10,3,5,5,2,2,2,2,2,2,2};
unsigned char melody_index = 0;
const unsigned char melody_len = 13;

void Tick() {
    unsigned char in0 = (~PINA)&0x1;
    if (in0 != A0) {
        A0 = in0;
        if (in0 && !on) on = 1;
    }
    if (on) {
        if (melody_index >= melody_len) {
            on = 0;
            melody_index = 0;
        } else if (count < periods[melody_index]) {
            set_PWM(freq[melody[melody_index]]);
            count++;
        } else {
            count = 0;
            melody_index++;
        }
    } else set_PWM(0);
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
    PWM_on();
    TimerSet(100);
    TimerOn();
    while (1) {
        Tick();
        while (!TimerFlag);
        TimerFlag = 0;
    }
    return 1;
}
