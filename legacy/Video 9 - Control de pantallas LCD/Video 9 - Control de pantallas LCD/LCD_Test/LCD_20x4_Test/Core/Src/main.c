#include "main.h"
#include "gpio.h"
#include "lcd.h"

#include <stdio.h>

uint16_t valor = 4096;
char buf_lcd[18];
const char fig_1[8] = {0x0A, 0x0A, 0x0A, 0x00, 0x11, 0x11, 0x0E, 0x00};
const char fig_2[8] = {0x04, 0x11, 0x0E, 0x04, 0x04, 0x0A, 0x11, 0x00};

void SystemClock_Config(void);

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	Lcd_Init();
	Lcd_CGRAM_CreateChar(0, fig_1);
	Lcd_CGRAM_CreateChar(1, fig_2);
	Lcd_Clear();

	while(1)
	{
		Lcd_Set_Cursor(1,3);
		Lcd_Send_String("Pantalla LCD 20x4");
		Lcd_Set_Cursor(2,3);
		Lcd_Send_String("Con STM32F103C8T6");
		Lcd_Set_Cursor(3,6);
		sprintf(buf_lcd, "Valor: %u", valor);
		Lcd_Send_String(buf_lcd);
		Lcd_Set_Cursor(4,4);
		Lcd_Send_String("CGRAM Char: ");
		Lcd_CGRAM_WriteChar(0);
		Lcd_CGRAM_WriteChar(1);
		HAL_Delay(2500);

		Lcd_Clear();
		HAL_Delay(500);
	}
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif
