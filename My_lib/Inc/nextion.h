/*
 * nextion.h
 *
 *  Created on: 24.06.2018
 *      Author: Aleksander Ostrowski
 */

#ifndef NEXTION_H_
#define NEXTION_H_

typedef struct{
	uint8_t flag;
	uint16_t time;
}tim;

unsigned char read_next_page();
unsigned char read_next_button_state(uint8_t page,uint8_t adr);
void reset_next_button_state(uint8_t page,uint8_t adr);
unsigned char read_res_next_button_state(uint8_t page,uint8_t adr);
unsigned char read_next_object_val(uint8_t page,uint8_t adr);
void read_next_loop();//w przerwaniu od RX odczytje ramki dancyh i wpisuje dane od tablicy
void read_next_init();//w STM32 musi być aby załączyć odbiornik
void read_next_error();//umieszcamy w pętli przerwania błędu UART
void tim_next(uint16_t *time,uint8_t *flag, uint8_t counter, uint8_t but_state);//w przerwaniu co 100 us
//liczy czas naciśnięcia przycisku i ustawia flage jeśli czas poprawny
unsigned char read_tim_next_flag(uint8_t *flag);// czyta i kasuje flage spełnonego warunku od czasu(tim_next)
//void reset_tim_next_flag(uint8_t *flag);

#endif /* NEXTION_H_ */
