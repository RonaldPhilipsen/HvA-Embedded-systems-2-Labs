/**
  ******************************************************************************
  * @file    ./Lab2/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to configure and use GPIOs through 
  *          the STM32L1xx HAL API.
  ******************************************************************************
  */

#include "main.h"
#include <stdlib.h>


void SystemClock_Config(void);

/**
  * @brief  Main program
  * @retval None
  */
int main(void)
{
	HAL_Init();
	SystemClock_Config();

	// Pointer to the RCC register
	const char* RCCp = (char*) 0x40023800;
	// Pointer to the GPIO B registers
	int* PBp = (int *) 0x40020400;

	// Enable GPIO A to H 
	*((int*)(RCCp + 0x1C)) |= 0x3f;

	//enable the system configuration controller cloc 
	*((int*)(RCCp + 0x20)) |= 1;

	// PBp has the value of the gpio B register with offset 0x00
	*PBp = 0x5000;


	// Set gpio 7 To high
	*(int*)(0x40020414) |= 0x80;
	// Set gpio 6 to low 
	*(int*)(0x40020414) &= ~0x40;

	uint delay = 1000000;
	int direction = -1000;
	int holdTime = 0;
	
	while (1)
	{
		if ((GPIOA->IDR & (uint16_t) 0x0001) != 0x0)
		{
			if (holdTime != 0)
			{	
				int nextStep = direction * (holdTime^2);
				if (abs(nextStep) > delay || delay > 1500000)
				{
					// if the time gets too low or too high, reverse direction
					direction *= -1;
					nextStep = direction * (holdTime ^ 2);
					}
				delay += nextStep;
			}

			holdTime++;
		}
		else
		{	
			holdTime = 0;
			delay = 1000000;
		}

		uint i = 0;
		while (i++ < delay)
		{	
			
			//Wait for i to reach 100000 Seconds
		}

		// Toggle both 7 and 6 by Xor-ing the value 
		*(int*)(0x40020414) ^= 0xC0;

	}
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 32000000
  *            HCLK(Hz)                       = 32000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 16000000
  *            PLLMUL                         = 6
  *            PLLDIV                         = 3
  *            Flash Latency(WS)              = 1
  * @retval None
  */
void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };

	/* Enable HSE Oscillator and Activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLMUL          = RCC_PLL_MUL6;
	RCC_OscInitStruct.PLL.PLLDIV          = RCC_PLL_DIV3;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		/* Initialization Error */
		while (1) ; 
	}

	/* Set Voltage scale1 as MCU will run at 32MHz */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
	/* Poll VOSF bit of in PWR_CSR. Wait until it is reset to 0 */
	while (__HAL_PWR_GET_FLAG(PWR_FLAG_VOS) != RESET) {}
	;

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
	{
		/* Initialization Error */
		while (1) ; 
	}
}
#ifdef  USE_FULL_ASSERTs

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	   ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	 /* Infinite loop */
	while (1)
	{
	}
}
#endif

