#include "main.h"
#include "discover_board.h"

static __IO uint32_t TimingDelay;
void Delay(__IO uint32_t nTime);

RCC_ClocksTypeDef RCC_Clocks;

void Config_Systick() {
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency/ 1000);
}

int main(void)
{
	SystemInit();
	Config_Systick();

	GPIO_Init(LD_GREEN);

	while (1)
	{
		GPIO_TOGGLE(GPIOB, LD_GREEN);
		/* Insert 100 ms delay */
		Delay(1000);
	}
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
	int x = 0;
	TimingDelay = nTime;

  while(TimingDelay != 0){
	  x= x+ 1;
  }
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
