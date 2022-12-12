/*
 * nextion.c
 *
 *  Created on: 24.06.2018
 *      Author: Aleksander Ostrowski
 */
#include "stm32f1xx_hal.h"
#include "usart.h"

#define Pages 3
#define ButAdresses 255
#define ValAdresses 10


#define PAGE_WRITE	NEXT.data[2]
#define VALUE		NEXT.data[2]
#define ADR_WRITE 	NEXT.data[3]
#define STATE		NEXT.data[4]
#define SET_UART_RX_REGISTER  HAL_UART_Receive_IT(&huart1,&NEXT.receive, 1)


typedef struct{
	uint8_t count;
	uint8_t page;
	uint8_t data[9];
	uint8_t but_state[Pages][ButAdresses];
	uint8_t val_state[Pages][ValAdresses];
	uint8_t receive;
	uint8_t adr;

}nextion;

nextion NEXT;

typedef struct{
	uint8_t flag;
	uint16_t time;
}tim;

unsigned char read_next_page()
{
	return NEXT.page;
}

unsigned char read_next_button_state(uint8_t page,uint8_t adr)// podajemy strone oraz adres przycisku
{
	return NEXT.but_state[page][adr];
}

void reset_next_button_state(uint8_t page,uint8_t adr)
{
	NEXT.but_state[page][adr]=0;
}

unsigned char read_res_next_button_state(uint8_t page,uint8_t adr)// podajemy strone oraz adres przycisku
{
	uint8_t a;

	a=NEXT.but_state[page][adr];
	NEXT.but_state[page][adr]=0;

	return a;
}

unsigned char read_next_object_val(uint8_t page,uint8_t adr)
{
	return NEXT.val_state[page][adr];
}

void read_next_loop()//petla odczytuje i zapisuje do tablicy pamieci stany i wartosci objektow wyswietlacza nextion
{

	NEXT.count++;
	//NEXT.data liczy od 1
	NEXT.data[NEXT.count]=NEXT.receive;

	//sprawdzenie czy nie ma błedu w odbiorze
	if(NEXT.data[1]!=0x71 && NEXT.data[1]!=0x65 && NEXT.data[1]!=0x66)
	{
		NEXT.count=0;
	}
	if((NEXT.data[1]==0x65) && (NEXT.data[5]==0xFF) &&
			(NEXT.data[6]==0xFF) && (NEXT.data[7]==0xFF) && (NEXT.count==7))
	{
		NEXT.page=PAGE_WRITE;
		NEXT.but_state[PAGE_WRITE][ADR_WRITE]=STATE;
		NEXT.adr=ADR_WRITE;

		NEXT.count=0;
		NEXT.data[1]=0;
	}
	if((NEXT.data[1]==0x71) && (NEXT.data[6]==0xFF) &&
			(NEXT.data[7]==0xFF) && (NEXT.data[8]==0xFF) && (NEXT.count==8))
	{
		NEXT.val_state[NEXT.page][NEXT.adr]=VALUE;

		NEXT.count=0;
		NEXT.data[1]=0;
	}

	if((NEXT.data[1]==0x66) && (NEXT.data[3]==0xFF) &&
			(NEXT.data[4]==0xFF) && (NEXT.data[5]==0xFF) && (NEXT.count==5))
	{
		NEXT.page=PAGE_WRITE;

		NEXT.count=0;
		NEXT.data[1]=0;
	}
	if(NEXT.count>8)
	{
		NEXT.count=0;
	}

	SET_UART_RX_REGISTER;
}

void read_next_init()
{
	SET_UART_RX_REGISTER;
}
void read_next_error()
{
	USART1->SR|=USART_SR_FE;
	NEXT.count=0;//kasuje licznik ramek
	SET_UART_RX_REGISTER;
}

void tim_next(uint16_t *time,uint8_t *flag, uint8_t counter, uint8_t but_state)//w przerwaniu co 10ms
{			//odlicza okreslony czas po nasisniecu danego przycisku i wystawia falage jesli jest speliony warunek
	if(but_state==1)
	{
		if(*time<counter)
		{
			static int c;
			c++;
			if(c>99)
			{
				c=0;
				(*time)++;
			}
		}
		else *flag=1;
	}
	else
	{
		*time=0;
	}
}

unsigned char read_tim_next_flag(uint8_t *flag)
{			//odczytuje fage timera od nacisnietego przycisku
	uint8_t a;

	a=*flag;
	*flag=0;

	return a;
}

void reset_tim_next_flag(uint8_t *flag)
{			//kasuje flage timera od nacisnietego przycisku
	*flag=0;
}







