/*
 * uart_lib.c
 *
 *  Created on: 27.09.2017
 *      Author: Aleksander Ostrowski
 */
#include "stm32f1xx_hal.h"

#define USART_Next	USART1
#define next			uart1

//uint8_t transmit,counter_tx,counter_map,val,val_map[20],adr,w_string;
//void write_uart_val(uint8_t tab[],uint8_t value,uint8_t count,uint8_t count_max)

/*$$$$$$$$$   OPIS ZMIENNYCH   $$$$$$$$$$
adr - okresla ktora dana z talicy tab[] ma sie wyslac
counter tx - licznik sluzacy do sprawdzania ktora czesc daych sie ma wyslac tab[] czy value i ktora z value(setki,jednostki,dziesiatki)
counter_map - licznik sluzacy do sprawdzenia ktory ciag danych jest wysylany 1,2,3 itd
val - dana przesylajace okreslina wartosc wpisana do bufora uart
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/

typedef struct
{

	uint8_t counter_tx;
	uint8_t counter_map;
	uint8_t val;
	uint8_t adr;
	uint8_t error_counter;
	uint8_t count_max;
	uint16_t val_map[20];
	uint8_t r_flag;

}uart;

uart uart1;




void clear_error_uart_next(void)
{
	next.error_counter=0;
}

void write_next_val(uint8_t tab[],uint16_t value,uint8_t count,uint8_t count_max)
{
	next.count_max=count_max;

		if(next.counter_map == count)//sprawdza ktora wartosc ma wyslac 1,2,3 itd
		{
			if(USART_Next->SR & USART_SR_TC)// czeka az bufor bedzie gotowy
			{
				clear_error_uart_next();//zerowanie licznika błędu

				if(next.counter_tx==0) //wysyla tablice tab[]
				{
					USART_Next->DR=tab[next.adr];
					next.adr++;
					if(tab[next.adr]==0)next.counter_tx++;// zwieksza counter_tx aby przejsc do nastpenej funkcji gdy juz wyslal cala tablice
				}
				else		//wysyla wartosc value przekonwertowana do ASCII
				{
					if(next.counter_tx==1){next.val=0x30 | (value/1000)%10;}
					if(next.counter_tx==2){next.val=0x30 | (value/100)%10;}
					if(next.counter_tx==3){next.val=0x30 | (value/10)%10;}
					if(next.counter_tx==4){next.val=0x30 | (value%10);}
					if(next.counter_tx>4)// wysyla 3 razy 0xff
					{
						next.val=0xff;
					}
					next.counter_tx++;
					if(next.counter_tx>60)//koniec wyslania zeruje wartosci
					{
						next.counter_tx=0;
						next.adr=0;
						next.counter_map++;
						next.val_map[count]=value;
						if(next.counter_map > count_max)next.counter_map=0;
							//po wyzerowaniu licznika ciagu wyslanych danych moze wyslaj inna data
					}
					if(next.counter_tx<8)USART_Next->DR=next.val;
				}
			}
			USART_Next->CR1|=USART_CR1_TCIE;
		}

}

void write_next_txt(uint8_t tab[],uint8_t count,uint8_t count_max)
{
	next.count_max=count_max;

		if(next.counter_map == count)//sprawdza ktora wartosc ma wyslac 1,2,3 itd
		{
			if(USART_Next->SR & USART_SR_TC)// czeka az bufor bedzie gotowy
			{
				clear_error_uart_next();//zerowanie licznika błędu

				if(next.counter_tx==0) //wysyla tablice tab[]
				{
					USART_Next->DR=tab[next.adr];
					next.adr++;
					if(tab[next.adr]==0)next.counter_tx++;// zwieksza counter_tx aby przejsc do nastpenej funkcji gdy juz wyslal cala tablice
				}
				else		//wysyla wartosc value przekonwertowana do ASCII
				{
					if(next.counter_tx>0)// wysyla 3 razy 0xff
					{
						next.val=0xff;
					}
					next.counter_tx++;
					if(next.counter_tx>60)//koniec wyslania zeruje wartosci
					{
						next.counter_tx=0;
						next.adr=0;
						next.counter_map++;
						if(next.counter_map > count_max)next.counter_map=0;
							//po wyzerowaniu licznika ciagu wyslanych danych moze wyslaj inna data
					}
					if(next.counter_tx<4)USART_Next->DR=next.val;
				}
			}
			USART_Next->CR1|=USART_CR1_TCIE;
		}

}

void error_void_uart_next(uint8_t presclaler)
{
	next.error_counter++;
	if(next.error_counter>presclaler)//8 bajtów musi się nie wysłać aby to się wykonało
	{
		next.counter_map++;
			if(next.counter_map > next.count_max)next.counter_map=0;

	}
}

void write_next_init(void)
{
	USART_Next->CR1|=USART_CRO1_TCIE;
}



//
//void clear_error_uart1(void)
//{
//	uart1.error_counter=0;
//}
//
//void write_uart_string(uint8_t tab[],uint8_t count,uint8_t count_max)
//{
//	uart1.count_max=count_max;
//    uart1.r_flag=1;
//		if(uart1.counter_map == count)//sprawdza ktora wartosc ma wyslac 1,2,3 itd
//		{
//			if(LPUART1->ISR & USART_ISR_TC)// czeka az bufor bedzie gotowy
//			{
//				clear_error_uart1();
//
//
//					LPUART1->TDR=tab[uart1.adr];
//					uart1.adr++;
//					if(tab[uart1.adr]==0)// zwieksza counter_tx aby przejsc do nastpenej funkcji gdy juz wyslal cala tablice
//					{
//						uart1.adr=0;
//						uart1.counter_map++;
//						uart1.r_flag=0;
//						//val_map[count]=value;
//						if(uart1.counter_map > count_max)uart1.counter_map=0;
//							//po wyzerowaniu licznika ciagu wyslanych danych moze wyslaj inna data
//					}
//			}
//		}
//
//}
//
//unsigned char reset_val(void)
//{
//	unsigned char a;
//
//	if(uart1.r_flag==0)a=0;//dla jednego stringa musi byc zmienna r_flag;
//	else a=1;
//
//	return a;
//}
//
//void error_void_uart1(void)
//{
//	uart1.error_counter++;
//	if(uart1.error_counter>20)//8 bajtów musi się nie wysłać aby to się wykonało
//	{
//			uart1.counter_map++;
//			if(uart1.counter_map > uart1.count_max)uart1.counter_map=0;
//
//	}
//}
