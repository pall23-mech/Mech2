/*
 * UART_Trans.h
 *
 *  Created on: Mar 3, 2024
 *      Author: palli
 *      transmits uart, converts some
 *      numbers reverses etc for correct display
 */

#ifndef UART_TRANS_H_
#define UART_TRANS_H_

// Send a string
void uart_sendString(const char* str);
// Function to convert an unsigned int to string and print
void uart_sendDecimal(unsigned int num);
// Utility function to reverse a string in place
void reverse(char* str, int len);
// Convert and send a number in hexadecimal format
void uart_sendHex(unsigned long num, unsigned char bytes);
// Transmit a single byte
void uart_transmit(unsigned char data);
// Initialize UART
void uart_init(void);

#endif /* UART_TRANS_H_ */
