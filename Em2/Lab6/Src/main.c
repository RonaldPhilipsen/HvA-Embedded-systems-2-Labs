#ifdef __cplusplus
extern "C"
#endif
#include "stm32l152c_discovery.h"
#include "stm32l152c_discovery_glass_lcd.h"
					  
void Init_GPIOs(void);
void Config_Systick(void);
void SystemClock_Config(void);

void DAC_Configuration(void);
void ADC_Configuration(void);
DAC_HandleTypeDef hdac;
ADC_HandleTypeDef hadc;

int main(void)
{
	HAL_Init();
	SystemClock_Config();

	BSP_LCD_GLASS_Init();
	BSP_LCD_GLASS_DisplayString((uint8_t*)"Lab 6");
	Init_GPIOs();
	DAC_Configuration();
	ADC_Configuration();

	uint16_t output = 0, input = 0;
	uint8_t turn = 0;

	HAL_DAC_Start(&hdac, DAC_CHANNEL_1);

	for (;;)
	{
		HAL_ADC_Stop(&hadc);
		HAL_Delay(2000);

		switch (turn) {
		case 0:
			output = 0;
			turn++;
			break;
		case 1:
			output = 2047;
			turn++;
			break;
		case 2:
			output = 4095;
			turn = 0;
			break;
		}

		HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, output);
		HAL_Delay(10);

		HAL_ADC_Start(&hadc);

		if (HAL_ADC_PollForConversion(&hadc, 1000) == HAL_OK) {
			input = HAL_ADC_GetValue(&hadc);
		}
		else {
			continue;
		}


		char displayString[20];

		// While User button pressed, show output
		if (GPIOA->IDR & USER_BUTTON_PIN) {
			sprintf(displayString, "%d", output);
			BSP_LCD_GLASS_DisplayString((uint8_t*) displayString);
			//delay the program indefinitely while the user button is pressed.
			while (GPIOA->IDR & USER_BUTTON_PIN) ;
		}
		
		sprintf(displayString, "%d", input);				
		BSP_LCD_GLASS_Clear();
		BSP_LCD_GLASS_DisplayString((uint8_t*) displayString);
	}
}

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

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

void Init_GPIOs()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = USER_BUTTON_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM; 
	HAL_GPIO_Init(USER_BUTTON_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = LED_GREEN | LED_BLUE;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM; 
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	HAL_GPIO_WritePin(GPIOB, LED_GREEN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, LED_BLUE, GPIO_PIN_RESET);

}

void DAC_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = GPIO_PIN_4;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	hdac.Instance = DAC1;
	__DAC_CLK_ENABLE();

	HAL_DAC_Init(&hdac);
}


void ADC_Configuration()
{
	GPIO_InitTypeDef gpioIS;

	__GPIOC_CLK_ENABLE();
	__ADC1_CLK_ENABLE();
			  
	gpioIS.Pin = GPIO_PIN_5;
	gpioIS.Mode = GPIO_MODE_ANALOG;
	gpioIS.Pull = GPIO_NOPULL;
	gpioIS.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOA, &gpioIS);
													 	
	HAL_NVIC_SetPriority(ADC1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(ADC1_IRQn);
													 
	ADC_ChannelConfTypeDef adcChannel;

	hadc.Instance = ADC1;
	hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
	hadc.Init.Resolution = ADC_RESOLUTION_12B;
	hadc.Init.ScanConvMode = DISABLE;
	hadc.Init.ContinuousConvMode = DISABLE;
	hadc.Init.DiscontinuousConvMode = DISABLE;
	hadc.Init.NbrOfDiscConversion = 0;
	hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc.Init.NbrOfConversion = 1;
	hadc.Init.DMAContinuousRequests = DISABLE;
	hadc.Init.EOCSelection = DISABLE;
	HAL_ADC_Init(&hadc);

	adcChannel.Channel = ADC_CHANNEL_4;
	adcChannel.Rank = 1;
	adcChannel.SamplingTime = ADC_SAMPLETIME_9CYCLES;

	if (HAL_ADC_ConfigChannel(&hadc, &adcChannel) != HAL_OK)
	{
		asm("bkpt 255");
	}

	
	__HAL_ADC_ENABLE(&hadc);
}



