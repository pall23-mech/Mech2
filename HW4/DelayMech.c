#include "DelayMech.h"

void delay_ms(unsigned long long ms) {
    unsigned long cycles = ms * 16000; // Correct calculation for 16MHz CPU, assuming 1 millisecond = 16000 cycles

    // Loop that decrements 'cycles' until it reaches 0
    while(cycles > 0){
        __asm__ __volatile__("nop"); // Perform a no-operation to ensure the loop body is not empty
        cycles -= 4; // Adjust this value based on actual calibration
    }
}
