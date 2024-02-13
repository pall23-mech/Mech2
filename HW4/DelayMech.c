#include "DelayMech.h"

void delay_ms(unsigned  long ms) {
    unsigned long pre = ms * (F_CPU/1000/34); //assumed 1ms = 16000 cycles, however it appears to be 1000 times faster
    unsigned long scale =(pre);    //scaling in attempt to prevent rounding errors
    unsigned long cycles=scale;
    // Loop that decrements 'cycles' until it reaches 0
    while(cycles > 0){
       cycles--;
    }}
void delay_us(unsigned long us) {
        unsigned long cycles = us*(F_CPU/1000000/66);///more efficient not to use cpu // Correct calculation for 16MHz CPU, assuming 1 millisecond = 16000 cycles
         //scaling
        // Loop that decrements 'cycles' until it reaches 0
        while(cycles > 0){
        	cycles--;
        }}
void delay_s(unsigned long s) {
   unsigned long pre = s * F_CPU/100;///168;//00/168; //*(/1,68); // Correct calculation for 16MHz CPU, assuming 1 millisecond = 16000 cycles
   unsigned long scale =200*pre;
   unsigned long cycles=scale/500;    // Loop that decrements 'cycles' until it reaches 0

   while(cycles > 0){
        __asm__ __volatile__("nop"); // Perform a no-operation to ensure the loop body is not empty
       cycles -= 4; // Adjust this value based on actual calibration

}
}
