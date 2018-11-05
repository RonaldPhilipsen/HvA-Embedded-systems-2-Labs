#include "main.h"
#include <stdlib.h>


void SystemClock_Config(void);
void Gpio_init(void);
void GameFinished(bool gameWon);
void handleChar(volatile double *  character, double gameSpeed, bool charLocked);


double numWins = 0;
int gameTicks = 0;

volatile double char1, char2, char3;
volatile bool lock1, lock2, lock3;

int main(void)
{

	HAL_Init();
	SystemClock_Config();
	BSP_LCD_GLASS_Init();
	Gpio_init();

	BSP_LED_Init(LED_BLUE);
	BSP_LED_Init(LED_GREEN);

	BSP_LCD_GLASS_ScrollSentence((uint8_t *) "       Lab 7: Ron", 1, SCROLL_SPEED_MEDIUM);
	BSP_LCD_GLASS_ScrollSentence((uint8_t *) "       Press  button to start", 1, SCROLL_SPEED_MEDIUM);					  	
	while (!(GPIOA->IDR & USER_BUTTON_PIN)) {
	}
								  
	char1 = (rand() % 100) / 10; 
	char2 = (rand() % 100) / 10;
	char3 = (rand() % 100) / 10;								   
	
	while (1)
	{	
		double gameSpeed = 0.5f + (numWins / 5);
		BSP_LCD_GLASS_BarLevelConfig(numWins);
											  											  
		if (lock1 && lock2 && lock3)
		{
			bool gameWon = ((int) char1 == (int) char2 && (int) char2 == (int) char3);			
			GameFinished(gameWon);
		}		 	   

		
		handleChar(&char1, gameSpeed, lock1);									
		handleChar(&char2, gameSpeed, lock2);		
		handleChar(&char3, gameSpeed, lock3);

		char displayString[20];
		sprintf(displayString, "%02d%02d%02d", (int) char1, (int) char2, (int) char3);
		BSP_LCD_GLASS_DisplayString((uint8_t*) displayString);
				   
		if (GPIOA->IDR & USER_BUTTON_PIN && ( gameTicks > 20))
		{
			if (!lock1)
			{
				lock1 = true;						
			}
			else if (!lock2)
			{
				lock2 = true;
			}

			else if (!lock3)
			{
				lock3 = true;
			}		   			
			while (GPIOA->IDR & USER_BUTTON_PIN) ;
		}
		gameTicks++;
	}
}

void handleChar(volatile double *  character, double gameSpeed, bool charLocked)
{
	if (charLocked)
	{
		return;
	}
	
	*character += gameSpeed;		 		 		

	if (*character > 10.5)
	{
		*character = 0;
	}			  
}


void GameFinished(bool gameWon)
{
	if (gameWon)
	{
		numWins++;
		BSP_LCD_GLASS_DisplayString((uint8_t *) "WINNER");

		BSP_LED_On(LED_BLUE);
		BSP_LED_Off(LED_GREEN);

		for (int i = 0; i < 10; i++)
		{
			BSP_LED_Toggle(LED_BLUE);
			BSP_LED_Toggle(LED_GREEN);
			HAL_Delay(500);

		}

	}
	else
	{																							
		BSP_LCD_GLASS_DisplayString((uint8_t *) "LOSER ");
		numWins = 0;

		BSP_LED_On(LED_BLUE);
		BSP_LED_On(LED_GREEN);

		for (int i = 0; i < 10; i++)
		{
			BSP_LED_Toggle(LED_BLUE);
			BSP_LED_Toggle(LED_GREEN);
			HAL_Delay(500);
		}
	}


	BSP_LCD_GLASS_ScrollSentence((uint8_t *) "Press button to start", 1, SCROLL_SPEED_MEDIUM);	

	while (!(GPIOA->IDR & USER_BUTTON_PIN)) {}
	gameTicks = 0;
}

void Gpio_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = USER_BUTTON_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(USER_BUTTON_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM; 
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM; 
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_4 | GPIO_PIN_5;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM; 
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
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
	/* User can add his own implementation to report the file name and line number,
	   ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	 /* Infinite loop */
	while (1)
	{
	}
}
#endif

