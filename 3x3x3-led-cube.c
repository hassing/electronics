/* Simple 3x3x3 LED cube animation code for PIC16.
 * Originally tested with PIC16F690. Pinout in "format" section below under includes.
 * Compiled with MPLAB XC.
 * 
 * https://hassing.org/3x3x3-led-cube
 * 
 * MIT License
 * 
 * Copyright (c) 2022 Lasse Hassing
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

// CONFIG
#pragma config FOSC = INTRCCLK  // Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select bit (MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Selection bits (BOR enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)

#define _XTAL_FREQ 8000000
#include <xc.h>

// FORMAT:   0bCCCCCCCCBBBBBBBB / 0b7654321076543210)
#define COL1 0b1100000000000000 // RC5 (connected to cathode on one column of 9 LEDS / LOW == ON)
#define COL2 0b1010000000000000 // RC6 (--||--)
#define COL3 0b0110000000000000 // RC7 (--||--)
#define R1   0b0000010000000000 // RC2 (Connected to anode of one row of 3 LEDS / HIGH == ON)
#define R2   0b0000100000000000 // RC3 (--||--)
#define R3   0b0001000000000000 // RC4 (--||--)
#define Y1   0b0000000010000000 // RB7 (--||--)
#define Y2   0b0000000100000000 // RC0 (--||--)
#define Y3   0b0000001000000000 // RC1 (--||--)
#define W1   0b0000000000010000 // RB4 (--||--)
#define W2   0b0000000000100000 // RB5 (--||--)
#define W3   0b0000000001000000 // RB6 (--||--)
#define NONE 0b0000000000000000 // Turn all lights off
// RA2 is a button that is HIGH when pressed.

signed char animation = 0;
__bit checkInput(void) {
    if(PORTA & 0b00000100) {
        PORTB = 0b00000000;
        PORTC = 0b11100000;

        animation += 1;
        if(animation > 5) {
            animation = 0;
        }
        
        while(PORTA & 0b00000100) {
            __delay_ms(100);
        }
        return 1;
    }
    return 0;
}

void showRow(unsigned int c1, unsigned int c2, unsigned int c3) {
    c1 = COL1 | c1;
    c2 = COL2 | c2;
    c3 = COL3 | c3;
    
    PORTB = (unsigned char)(c1);
    PORTC = (unsigned char)(c1 >> 8);
    __delay_us(125);
    
    PORTB = (unsigned char)(c2);
    PORTC = (unsigned char)(c2 >> 8);
    __delay_us(125);
    
    PORTB = (unsigned char)(c3);
    PORTC = (unsigned char)(c3 >> 8);
    __delay_us(125);
}

void animation1() {
    unsigned int c = 0;
    unsigned int l = 100;
    
    while(c++ < l) {
        showRow(W1 | Y2 | R3,
                W2 | Y2 | R2,
                W3 | Y2 | R1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(W2 | Y3,
                W2 | Y2 | R1 | R2 | R3,
        W2 | Y1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(W3 | R1,
                W2 | Y1 | Y2 | Y3 | R2,
                W1 | R3);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(Y1 | R2,
                W2 | W1 | W3 | Y2 | R2,
                Y3 | R2);
    }
    c = 0; if(checkInput()) { return; }
}

void animation2() {
    unsigned int c = 0;
    unsigned int l = 100;
    
    while(c++ < l) {
        showRow(NONE,
                NONE,
                NONE);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(NONE,
                R2,
                NONE);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R2,
                R2,
                NONE);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R2 | R3,
                R2,
                NONE);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R2 | R3,
                R2 | R3,
                NONE);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R2 | R3,
                R2 | R3,
                R3);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R2 | R3,
                R2 | R3,
                R3 | R2);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R2 | R3,
                R2 | R3,
                R3 | R2 | R1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R2 | R3,
                R2 | R3 | R1,
                R3 | R2 | R1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R2 | R3 | R1,
                R2 | R3 | R1,
                R3 | R2 | R1);
    }
    c = 0; if(checkInput()) { return; }
    


    while(c++ < l) {
        showRow(R2 | R3 | R1,
                R2 | R3 | R1 | Y2,
                R3 | R2 | R1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R2 | R3 | R1 | Y2,
                R2 | R3 | R1 | Y2,
                R3 | R2 | R1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R2 | R3 | R1 | Y2 | Y3,
                R2 | R3 | R1 | Y2,
                R3 | R2 | R1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R2 | R3 | R1 | Y2 | Y3,
                R2 | R3 | R1 | Y2 | Y3,
                R3 | R2 | R1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R2 | R3 | R1 | Y2 | Y3,
                R2 | R3 | R1 | Y2 | Y3,
                R3 | R2 | R1 | Y3);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R2 | R3 | R1 | Y2 | Y3,
                R2 | R3 | R1 | Y2 | Y3,
                R3 | R2 | R1 | Y3 | Y2);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R2 | R3 | R1 | Y2 | Y3,
                R2 | R3 | R1 | Y2 | Y3,
                R3 | R2 | R1 | Y3 | Y2 | Y1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R2 | R3 | R1 | Y2 | Y3,
                R2 | R3 | R1 | Y2 | Y3 | Y1,
                R3 | R2 | R1 | Y3 | Y2 | Y1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R2 | R3 | R1 | Y2 | Y3 | Y1,
                R2 | R3 | R1 | Y2 | Y3 | Y1,
                R3 | R2 | R1 | Y3 | Y2 | Y1);
    }
    c = 0; if(checkInput()) { return; }
    


    while(c++ < l) {
        showRow(R2 | R3 | R1 | Y2 | Y3 | Y1,
                R2 | R3 | R1 | Y2 | Y3 | Y1 | W2,
                R3 | R2 | R1 | Y3 | Y2 | Y1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R2 | R3 | R1 | Y2 | Y3 | Y1 | W2,
                R2 | R3 | R1 | Y2 | Y3 | Y1 | W2,
                R3 | R2 | R1 | Y3 | Y2 | Y1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R2 | R3 | R1 | Y2 | Y3 | Y1 | W2 | W3,
                R2 | R3 | R1 | Y2 | Y3 | Y1 | W2,
                R3 | R2 | R1 | Y3 | Y2 | Y1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R2 | R3 | R1 | Y2 | Y3 | Y1 | W2 | W3,
                R2 | R3 | R1 | Y2 | Y3 | Y1 | W2 | W3,
                R3 | R2 | R1 | Y3 | Y2 | Y1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R2 | R3 | R1 | Y2 | Y3 | Y1 | W2 | W3,
                R2 | R3 | R1 | Y2 | Y3 | Y1 | W2 | W3,
                R3 | R2 | R1 | Y3 | Y2 | Y1 | W3);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R2 | R3 | R1 | Y2 | Y3 | Y1 | W3 | W2,
                R2 | R3 | R1 | Y2 | Y3 | Y1 | W2 | W3,
                R3 | R2 | R1 | Y3 | Y2 | Y1 | W2 | W3);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R2 | R3 | R1 | Y2 | Y3 | Y1 | W3 | W2,
                R2 | R3 | R1 | Y2 | Y3 | Y1 | W2 | W3,
                R3 | R2 | R1 | Y3 | Y2 | Y1 | W2 | W3 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R2 | R3 | R1 | Y2 | Y3 | Y1 | W3 | W2,
                R2 | R3 | R1 | Y2 | Y3 | Y1 | W2 | W3 | W1,
                R3 | R2 | R1 | Y3 | Y2 | Y1 | W2 | W3 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R2 | R3 | R1 | Y2 | Y3 | Y1 | W3 | W2 | W1,
                R2 | R3 | R1 | Y2 | Y3 | Y1 | W2 | W3 | W1,
                R3 | R2 | R1 | Y3 | Y2 | Y1 | W2 | W3 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    
    
    while(c++ < l) {
        showRow(R2 | R3 | R1 | Y2 | Y3 | Y1 | W3 | W2 | W1,
                R3 | R1 | Y2 | Y3 | Y1 | W2 | W3 | W1,
                R3 | R2 | R1 | Y3 | Y2 | Y1 | W2 | W3 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R3 | R1 | Y2 | Y3 | Y1 | W3 | W2 | W1,
                R3 | R1 | Y2 | Y3 | Y1 | W2 | W3 | W1,
                R3 | R2 | R1 | Y3 | Y2 | Y1 | W2 | W3 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R1 | Y2 | Y3 | Y1 | W3 | W2 | W1,
                R3 | R1 | Y2 | Y3 | Y1 | W2 | W3 | W1,
                R3 | R2 | R1 | Y3 | Y2 | Y1 | W2 | W3 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R1 | Y2 | Y3 | Y1 | W3 | W2 | W1,
                R1 | Y2 | Y3 | Y1 | W2 | W3 | W1,
                R3 | R2 | R1 | Y3 | Y2 | Y1 | W2 | W3 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R1 | Y2 | Y3 | Y1 | W3 | W2 | W1,
                R1 | Y2 | Y3 | Y1 | W2 | W3 | W1,
                R2 | R1 | Y3 | Y2 | Y1 | W2 | W3 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R1 | Y2 | Y3 | Y1 | W3 | W2 | W1,
                R1 | Y2 | Y3 | Y1 | W2 | W3 | W1,
                R1 | Y3 | Y2 | Y1 | W2 | W3 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R1 | Y2 | Y3 | Y1 | W3 | W2 | W1,
                R1 | Y2 | Y3 | Y1 | W2 | W3 | W1,
                Y3 | Y2 | Y1 | W2 | W3 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R1 | Y2 | Y3 | Y1 | W3 | W2 | W1,
                Y2 | Y3 | Y1 | W2 | W3 | W1,
                Y3 | Y2 | Y1 | W2 | W3 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(Y2 | Y3 | Y1 | W3 | W2 | W1,
                Y2 | Y3 | Y1 | W2 | W3 | W1,
                Y3 | Y2 | Y1 | W2 | W3 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(Y2 | Y3 | Y1 | W3 | W2 | W1,
                Y3 | Y1 | W2 | W3 | W1,
                Y3 | Y2 | Y1 | W2 | W3 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(Y3 | Y1 | W3 | W2 | W1,
                Y3 | Y1 | W2 | W3 | W1,
                Y3 | Y2 | Y1 | W2 | W3 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(Y1 | W3 | W2 | W1,
                Y3 | Y1 | W2 | W3 | W1,
                Y3 | Y2 | Y1 | W2 | W3 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(Y1 | W3 | W2 | W1,
                Y1 | W2 | W3 | W1,
                Y3 | Y2 | Y1 | W2 | W3 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(Y1 | W3 | W2 | W1,
                Y1 | W2 | W3 | W1,
                Y2 | Y1 | W2 | W3 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(Y1 | W3 | W2 | W1,
                Y1 | W2 | W3 | W1,
                Y1 | W2 | W3 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(Y1 | W3 | W2 | W1,
                Y1 | W2 | W3 | W1,
                W2 | W3 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(Y1 | W3 | W2 | W1,
                W2 | W3 | W1,
                W2 | W3 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(W3 | W2 | W1,
                W2 | W3 | W1,
                W2 | W3 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(W3 | W2 | W1,
                W3 | W1,
                W2 | W3 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(W3 | W1,
                W3 | W1,
                W2 | W3 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(W1,
                W3 | W1,
                W2 | W3 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(W1,
                W1,
                W2 | W3 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(W1,
                W1,
                W2 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(W1,
                W1,
                W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(W1,
                W1,
                NONE);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(W1,
                NONE,
                NONE);
    }
    c = 0; if(checkInput()) { return; }
}

void animation3() {
    unsigned int c = 0;
    unsigned int l = 100;
    
    while(c++ < l) {
        showRow(R1 | R2 | R3 | W1,
                R1 | R2 | R3 | Y2 | W2 | Y3,
                R1 | R2 | Y3 | W3 | Y1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R1 | R2 | R3 | Y2,
                R1 | R2 | R3,
                R1 | R2 | R3 | Y1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R1 | R2 | R3 | Y2 | Y3,
                R1 | R2 | R3 | Y3,
                R2 | R3 | Y1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R1 | R2 | R3 | Y2 | Y3,
                R1 | R2 | R3 | Y3 | Y1,
                R1 | R2 | R3 | Y1 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R1 | R2 | Y2 | Y3 | W2,
                R1 | R2 | R3 | Y3 | Y1 | W3,
                R1 | R2 | R3 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R1 | R2 | R3 | Y3 | W3,
                R1 | R2 | R3 | Y1 | W3 | W1,
                R1 | R2 | R3 | Y3);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R1 | R2 | W3 | Y2,
                R1 | R2 | R3 | W1,
                R1 | R2 | R3 | Y3 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R2 | R3 | Y2 | W3,
                R1 | R2 | R3 | Y3,
                R1 | R2 | R3 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R1 | R2 | R3 | Y2 | Y1,
                R1 | R2 | R3 | W3 | Y2,
                R1 | R2 | R3 | Y3);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R1 | R2 | R3 | Y2 | Y1 | W2,
                R1 | R2 | R3 | Y2 ,
                R1 | R2 | Y3 | W3);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R1 | R2 | R3 | W1,
                R1 | R2 | R3 | Y2 | W2 | Y3,
                R1 | R2 | Y3 | W3 | Y1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R1 | R2 | R3 | Y1,
                R1 | R2 | R3 | Y3 | W3,
                R1 | R2 | Y3 | Y1);
    }
    c = 0; if(checkInput()) { return; }
    
    
    
    while(c++ < l) {
        showRow(R1 | R2 | R3 | Y1,
                R1 | R2 | R3,
                R1 | R2 | R3 | Y2);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R1 | R2 | R3 | Y1 | Y3,
                R1 | R2 | R3 | Y1,
                R2 | R3 | Y1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R1 | R2 | R3 | Y1 | Y3,
                R1 | R2 | R3 | Y1 | Y2,
                R1 | R2 | R3 | Y1 | W2);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R1 | R2 | Y2 | Y3 | W2,
                R1 | R2 | R3 | Y1 | W2,
                R1 | R2 | R3 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R1 | R2 | R3 | Y3 | W3,
                R1 | R2 | R3 | Y3 | W2 | W1,
                R1 | R2 | R3 | Y1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R1 | R2 | W3 | Y2,
                R1 | R2 | R3 | W3,
                R1 | R2 | R3 | Y1 | W1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R2 | R3 | Y2 | W2,
                R1 | R2 | R3 | Y1,
                R1 | R2 | R3 | Y3);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R1 | R2 | R3 | Y1,
                R1 | R2 | R3 | W3 | Y2,
                R1 | R2 | R3 | W3);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R1 | R2 | R3 | Y2 | Y3 | W1,
                R1 | R2 | R3 | Y2 ,
                R1 | R2 | Y3 | W3);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R1 | R2 | R3 | W3,
                R1 | R2 | R3 | W2 | Y3,
                R1 | R2 | Y3 | Y1);
    }
    c = 0; if(checkInput()) { return; }
    
    while(c++ < l) {
        showRow(R1 | R2 | R3 | Y1,
                R1 | R2 | R3 | Y3 | W3,
                R1 | R2 | Y3 | W1);
    }
    c = 0; if(checkInput()) { return; }
}

void animation4() {
    unsigned int c = 0;
    
    for(short col = 0; col < 4; col++) {
        for(short i = 8; i >= 0; i--) {
            while(c++ < (9-i)*3) {
                showRow(R1 | R2 | R3 | Y1 | Y2 | Y3 | W1 | W2 | W3,
                        R1 | R2 | R3 | Y1 | Y2 | Y3 | W1 | W2 | W3,
                        R1 | R2 | R3 | Y1 | Y2 | Y3 | W1 | W2 | W3);

                PORTB = 0b00000000;
                PORTC = 0b11100000;
                for(int x=0; x<i*i; x++) {
                    __delay_us(20);
                }
            }
            c = 0; if(checkInput()) { return; }
        }

        for(short i = 0; i <= 8; i++) {
            while(c++ < (9-i)*3) {
                showRow(R1 | R2 | R3 | Y1 | Y2 | Y3 | W1 | W2 | W3,
                        R1 | R2 | R3 | Y1 | Y2 | Y3 | W1 | W2 | W3,
                        R1 | R2 | R3 | Y1 | Y2 | Y3 | W1 | W2 | W3);
                
                PORTB = 0b00000000;
                PORTC = 0b11100000;
                for(int x=0; x<i*i; x++) {
                    __delay_us(20);
                }
            }
            c = 0; if(checkInput()) { return; }
        }
    }
}

void static_mode() {
    unsigned int c = 0;
    unsigned int l = 100;
        
    while(c++ < l) {
        showRow(R2 | R3 | R1 | Y2 | Y3 | Y1 | W3 | W2 | W1,
                R2 | R3 | R1 | Y2 | Y3 | Y1 | W2 | W3 | W1,
                R3 | R2 | R1 | Y3 | Y2 | Y1 | W2 | W3 | W1);
    }
    c = 0; if(checkInput()) { return; }
}

void off_mode() {
    checkInput();
}

void main(void) {
    ANSEL = 0x00;
    TRISA = 0b00000100;
    TRISB = 0b00000000;
    TRISC = 0b00000000;

    while(1) {
        if(animation == 0) {
            animation1();
        } else if(animation == 1) {
            animation2();
        } else if(animation == 2) {
            animation3();
        } else if(animation == 3) {
            animation4();
        } else if(animation == 4) {
            static_mode(); 
        } else {
            off_mode();
        }
    }
    
    return;
}
