/*
 * uart_lib.h
 *
 *  Created on: 27.09.2017
 *      Author: Aleksander Ostrowski
 */

#ifndef UART_LIB_H_
#define UART_LIB_H_

void write_next_val(uint8_t tab[],uint16_t value,uint8_t count,uint8_t count_max);
void write_next_txt(uint8_t tab[],uint8_t count,uint8_t count_max);
void error_void_uart_next(uint8_t presclaler);
void clear_error_uart_next(void);
void write_next_init(void);
//void write_uart_string(uint8_t tab[],uint8_t count,uint8_t count_max);
//void error_void_uart1(void);
//unsigned char reset_val(void);
//void clear_error_uart1(void);


#endif /* UART_LIB_H_ */
