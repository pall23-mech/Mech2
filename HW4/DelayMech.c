#include "DelayMech.h"

void delay_ms(unsigned long long ms) {
    unsigned long long pre = ms * F_CPU/100000; //assumed 1ms = 160 cycles
    unsigned long long scale =200*pre;    //scaling in attempt to prevent rounding errors
    unsigned long long cycles=scale/500;
    // Loop that decrements 'cycles' until it reaches 0
    while(cycles > 0){
        __asm__ __volatile__("nop"); // assuming 4 operations
        cycles -= 4; // 
    }}
void delay_us(unsigned long us) {
        unsigned long pre = us;///168;//00/168; //*(/1,68); // Correct calculation for 16MHz CPU, assuming 1 millisecond = 16000 cycles
        unsigned long cycles =(pre*200)/440*200/480;
       // unsigned long long uscale=scale/1000;
       // unsigned long long cycles=uscale/500;
        // Loop that decrements 'cycles' until it reaches 0
        while(cycles > 0){
        	cycles--;
        }}
void delay_s(unsigned long long s) {
   unsigned long long pre = s * F_CPU/100;///168;//00/168; //*(/1,68); // Correct calculation for 16MHz CPU, assuming 1 millisecond = 16000 cycles
   unsigned long long scale =200*pre;
   unsigned long long cycles=scale/500;    // Loop that decrements 'cycles' until it reaches 0

   while(cycles > 0){
        __asm__ __volatile__("nop"); // Perform a no-operation to ensure the loop body is not empty
       cycles -= 4; // Adjust this value based on actual calibration

}
}
