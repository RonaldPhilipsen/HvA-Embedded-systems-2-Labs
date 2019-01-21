/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_hal.h"
#include "stm32l152c_discovery.h"			
#include "stm32l152c_discovery_glass_lcd.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor COMPx instance used and associated 
   resources */
/* Definition for COMPx clock resources */
#define COMPx_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define COMPx_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOA_CLK_DISABLE()

/* Definition for COMPx Channel Pin */
#define COMPx_PIN                      GPIO_PIN_1
#define COMPx_GPIO_PORT                GPIOA 

/* Definition for COMPx's NVIC */
#define COMPx_IRQn                     COMP_IRQn
#define COMPx_IRQHandler               COMP_IRQHandler

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
