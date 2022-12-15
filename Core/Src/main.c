/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "uart_lib.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t pageset,flag;
uint16_t counter;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void odczyt_spi(uint8_t adres, uint8_t *dane);
void zapis_spi(uint8_t adres, uint8_t dane);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2)//10ms
	{
		error_void_uart_next(1);
		counter++;
		if(counter>999)
		{
			counter=0;
			flag=1;
		}
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_SPI2_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  uint8_t tx_buffer[64];
  uint16_t size = 0;
  uint8_t bufor_lsb;
  uint8_t bufor_msb;
  uint16_t raw=0;
  double Temp;
  char cels=248;

  HAL_TIM_Base_Start_IT(&htim2);

  write_next_init();

  zapis_spi(0x80,0b11110011);	//zapis do rejestru configuration


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  uint16_t t_main,t_modulo;

	  odczyt_spi(0x02,&bufor_lsb);
	  odczyt_spi(0x01,&bufor_msb);
	  raw = (bufor_lsb>>1) + (bufor_msb << 7);

	  if(flag==1)
	  {
	  	  Temp=(((double)raw*10)/32)-2380;
	  	  t_main=((uint16_t)Temp)/10;
	  	  t_modulo=((uint16_t)Temp)%10;
	  	  flag=0;
	  }
//	  size = sprintf((char*)tx_buffer, "%3.1f%cC\n", Temp, cels);
//	  //HAL_UART_Transmit(&huart1, tx_buffer, size, 1000);
//	  HAL_Delay(400);


	  uint8_t max_val=3;
	  write_next_val("pageset.val=", pageset ,     0, max_val);
	  write_next_val("n0.val=", t_main,     1, max_val);
	  write_next_val("n1.val=", t_modulo,     2, max_val);
	  write_next_val("j0.val=", Temp/10,     3, max_val);


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void odczyt_spi(uint8_t adres, uint8_t *dane)
{
    HAL_GPIO_WritePin(SPI_CS_GPIO_Port,SPI_CS_Pin,GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi2, &adres, 1, 100);
    HAL_SPI_Receive(&hspi2, dane, 1, 100);
    HAL_GPIO_WritePin(SPI_CS_GPIO_Port,SPI_CS_Pin,GPIO_PIN_SET);
}
void zapis_spi(uint8_t adres, uint8_t dane)
{
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port,SPI_CS_Pin,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, &adres, 1, 100);
	HAL_SPI_Transmit(&hspi2, &dane, 1, 100);
	HAL_GPIO_WritePin(SPI_CS_GPIO_Port,SPI_CS_Pin,GPIO_PIN_SET);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
