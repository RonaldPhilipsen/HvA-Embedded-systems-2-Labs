
#include <stdint.h>
#include "stm32l152c_discovery.h"
#include "stm32l152c_discovery_glass_lcd.h"

void Init_GPIOs(void);
void Config_Systick(void);
void SystemClock_Config(void);

void DAC_Configuration(void);
void ConvertDAC(uint16_t);
void ADC_Configuration(void);
uint16_t readADC1(void);

DAC_HandleTypeDef hdac;
ADC_HandleTypeDef hadc;

int main(void)
{
	HAL_Init();
	SystemClock_Config();

	BSP_LCD_GLASS_Init();
	//BSP_LCD_GLASS_DisplayString((uint8_t*)"Lab 6");
	Init_GPIOs();
	DAC_Configuration();
	ADC_Configuration();

	//BSP_LED_Init(LED_BLUE);
	//BSP_LED_Init(LED_GREEN);

	HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
	ConvertDAC(0);

	for (;;)
	{
		//BSP_LED_On(LED_GREEN); 		
		HAL_Delay(500);
		//BSP_LED_Off(LED_GREEN); 		
		HAL_Delay(500);
	}
}

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

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

	HAL_DAC_Init(&hdac);

	DAC->CR |= 0x38;
}
void ConvertDAC(uint16_t value)
{
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, value);
	DAC->CR |= 0x1;
}

void ADC_Configuration()
{
	GPIO_InitTypeDef gpioIS;
	gpioIS.Pin = GPIO_PIN_5;
	gpioIS.Mode = GPIO_MODE_ANALOG;
	gpioIS.Pull = GPIO_NOPULL;
	gpioIS.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &gpioIS);

	ADC_InitTypeDef adcIS;
	adcIS.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
	adcIS.Resolution = ADC_RESOLUTION_12B;
	adcIS.DataAlign = ADC_DATAALIGN_RIGHT;
	adcIS.ScanConvMode = ADC_SCAN_DISABLE;
	adcIS.ChannelsBank = ADC_CHANNELS_BANK_A;
	adcIS.ContinuousConvMode = DISABLE;
	adcIS.ExternalTrigConv = ADC_EXTERNALTRIGCONVEDGE_NONE;
	
	hadc.Init = adcIS;

	HAL_ADC_Init(&hadc);
	HAL_ADC_Start(&hadc);
}