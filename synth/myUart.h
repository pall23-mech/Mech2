/*
 * myUart.h
 *
 *  Created on: Apr 15, 2024
 *      Author: palli
 */

#ifndef MYUART_H_
#define MYUART_H_
unsigned char uart_receive(void);
void uart_init(void);
void uart_transmit(unsigned char data);


#endif /* MYUART_H_ */
