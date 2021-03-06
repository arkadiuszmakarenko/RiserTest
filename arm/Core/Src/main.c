/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_host.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "dwt_delay.h"
#include "amiga.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 HID_USBDevicesTypeDef* usb;
 HID_KEYBD_Info_TypeDef *k_pinfo;




/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef huart5;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_UART5_Init(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */



static void usb_keyboard_led(USBH_HandleTypeDef *usbhost, keyboard_led_t ld)
{
	keyboard_led_t led = ld;
	USBH_StatusTypeDef status;
	int retrygood = 1;
	if (usbhost != NULL)
	{
		for (;;)
		{
			status = USBH_HID_SetReport(usbhost, 0x02, 0x00, &led, 1);
			if (status == USBH_OK)
				retrygood--;
			if (retrygood <= 0)
				break;
		}
	}
}

static void usb_keyboard_led_init(USBH_HandleTypeDef * usbhost)
{
	keyboard_led_t led = CAPS_LOCK_LED | NUM_LOCK_LED | SCROLL_LOCK_LED;
	int n;
	if (usbhost != NULL)
	{
		for (n = 0; n < 2; n++)
		{
			usb_keyboard_led(usbhost, led);
			HAL_Delay(250);
			usb_keyboard_led(usbhost, 0);
			HAL_Delay(125);
		}
		/* Default leds off */
		usb_keyboard_led(usbhost, 0);
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
	led_status_t stat;
	uint8_t do_led = 0;
	uint8_t KeyboardLedInit = 0;
	static keyboard_led_t keyboard_led = 0;

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
  MX_UART5_Init();
  MX_USB_HOST_Init();
  /* USER CODE BEGIN 2 */
  DWT_Init();

  amikb_startup();
  amikb_ready(0);

  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(KBD_CLOCK_GPIO_Port, KBD_CLOCK_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(KBD_DATA_GPIO_Port, KBD_DATA_Pin, GPIO_PIN_SET);
  keyboard_code_t keycode = {0};





  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {


    /* USER CODE END WHILE */
    MX_USB_HOST_Process();

    /* USER CODE BEGIN 3 */

       usb = (HID_USBDevicesTypeDef*) USBH_HID_GetUSBDev();


    if (usb->keyboardusbhost!=NULL)
    {
    	if (KeyboardLedInit==0)
    	{
    		usb_keyboard_led_init(usb->keyboardusbhost);
    		//restore LED status on new keyboard.
    		usb_keyboard_led(usb->keyboardusbhost, keyboard_led);
    		KeyboardLedInit++;
    	}


    	k_pinfo = usb->keyboard ;
    	int i = 0;

    	if (usb!=NULL && k_pinfo != NULL)
    	{
     		keycode.lctrl = k_pinfo->lctrl;
    		keycode.lshift = k_pinfo->lshift;
    		keycode.lalt = k_pinfo->lalt;
    		keycode.lgui = k_pinfo->lgui;
    		keycode.rctrl = k_pinfo->rctrl;
    		keycode.rshift = k_pinfo->rshift;
    		keycode.ralt = k_pinfo->ralt;
    		keycode.rgui = k_pinfo->rgui;
    		for (i = 0; i < KEY_PRESSED_MAX; i++)
    		{
    			keycode.keys[i] = k_pinfo->keys[i];
    		}
    		HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
    		stat = amikb_process(&keycode);


    		switch (stat)
    							{
    								case LED_CAPS_LOCK_OFF:
    									keyboard_led &= ~CAPS_LOCK_LED;
    									do_led = 1;
    									break;
    								case LED_CAPS_LOCK_ON:
    									keyboard_led |= CAPS_LOCK_LED;
    									do_led = 1;
    									break;
    								case LED_NUM_LOCK_OFF:
    									keyboard_led &= ~NUM_LOCK_LED;
    									do_led = 1;
    									break;
    								case LED_NUM_LOCK_ON:
    									keyboard_led |= NUM_LOCK_LED;
    									do_led = 1;
    									break;
    								case LED_SCROLL_LOCK_OFF:
    									keyboard_led &= ~SCROLL_LOCK_LED;
    									do_led = 1;
    									break;
    								case LED_SCROLL_LOCK_ON:
    									keyboard_led |= SCROLL_LOCK_LED;
    									do_led = 1;
    									break;
    								case LED_RESET_BLINK:
    									usb_keyboard_led_init(usb->keyboardusbhost);
    									do_led = 0;
    									break;
    								default:
    								case NO_LED:
    									do_led = 0;
    									break;
    							}
    							// ...and let the led management to be done!
    							if (do_led)
    							{
    								usb_keyboard_led(usb->keyboardusbhost, keyboard_led);
    							}

    	}
    }
    else
    {
    	//Keyboard is deinit.

    	KeyboardLedInit = 0;

    }
    //clear usbDevHost
    usb->keyboardusbhost = NULL;



    if(usb->mouse!=NULL)
    {
    	HAL_GPIO_WritePin(INTSIG1_GPIO_Port, INTSIG1_Pin, GPIO_PIN_SET);
    	DWT_Delay(1);
    	HAL_GPIO_WritePin(INTSIG1_GPIO_Port, INTSIG1_Pin, GPIO_PIN_RESET);

    }




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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_UART5|RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.Uart5ClockSelection = RCC_UART5CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  huart5.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart5.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, INTSIG8_Pin|RW_Pin|DSACK0_Pin|FIRE1_Pin
                          |GPIO_PIN_1|GPIO_PIN_2|INTSIG4_Pin|INTSIG5_Pin
                          |INTSIG6_Pin|INTSIG7_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(KBD_DATA_GPIO_Port, KBD_DATA_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|DKWE_Pin|FIRE0_Pin
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|DKRD_Pin
                          |DKWD_Pin|GPIO_PIN_9|GPIO_PIN_10|INTSIG3_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, D0_Pin|D1_Pin|D2_Pin|GPIO_PIN_10
                          |LED2_Pin|D3_Pin|D4_Pin|D5_Pin
                          |D6_Pin|D7_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(KBD_CLOCK_GPIO_Port, KBD_CLOCK_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : INTSIG8_Pin RW_Pin DSACK0_Pin FIRE1_Pin
                           PC1 PC2 INTSIG4_Pin INTSIG5_Pin
                           INTSIG6_Pin INTSIG7_Pin */
  GPIO_InitStruct.Pin = INTSIG8_Pin|RW_Pin|DSACK0_Pin|FIRE1_Pin
                          |GPIO_PIN_1|GPIO_PIN_2|INTSIG4_Pin|INTSIG5_Pin
                          |INTSIG6_Pin|INTSIG7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : KBD_DATA_Pin */
  GPIO_InitStruct.Pin = KBD_DATA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(KBD_DATA_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 DKWE_Pin FIRE0_Pin
                           PA4 PA5 PA6 DKRD_Pin
                           DKWD_Pin PA9 PA10 INTSIG3_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|DKWE_Pin|FIRE0_Pin
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|DKRD_Pin
                          |DKWD_Pin|GPIO_PIN_9|GPIO_PIN_10|INTSIG3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : A0_Pin A1_Pin A2_Pin */
  GPIO_InitStruct.Pin = A0_Pin|A1_Pin|A2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : D0_Pin D1_Pin D2_Pin D3_Pin
                           D4_Pin D5_Pin D6_Pin D7_Pin */
  GPIO_InitStruct.Pin = D0_Pin|D1_Pin|D2_Pin|D3_Pin
                          |D4_Pin|D5_Pin|D6_Pin|D7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB10 LED2_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_10|LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : KBD_CLOCK_Pin */
  GPIO_InitStruct.Pin = KBD_CLOCK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(KBD_CLOCK_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : INTSIG2_Pin */
  GPIO_InitStruct.Pin = INTSIG2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(INTSIG2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : INTSIG1_Pin */
  GPIO_InitStruct.Pin = INTSIG1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(INTSIG1_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(HAL_GPIO_ReadPin(RW_GPIO_Port, RW_Pin))
	{
	GPIOB->OTYPER = 0x00; //Set up Data GPIOB in pull-push mode.
	GPIOB->BSRR = 0x0000FFFF; //Set Data pins to GPIO_PIN_SET

	//I had DSACK0 reconfigure using registers to pushpull and set up to 1
	//as it was not working I deleted it ...




	GPIOB->OTYPER = 0xFF; //Set up data GPIOB in open-drain mode.
	GPIOB->BSRR = 0x00000000; ////Set Data pins to default state
	}

    // This was to check
	//if (GPIO_Pin == INTSIG2_Pin) //Process RTC
	//	{

		//	uint inter = HAL_GPIO_ReadPin(A4_GPIO_Port, A4_Pin);
		//	uint8_t address = 0;
			//uint8_t _RW = HAL_GPIO_ReadPin(RW_GPIO_Port, RW_Pin);
		//	address =  HAL_GPIO_ReadPin(A5_GPIO_Port, A5_Pin);
		//	address = (address<<1)|HAL_GPIO_ReadPin(A4_GPIO_Port, A4_Pin);
	//		address = (address<<1)|HAL_GPIO_ReadPin(A3_GPIO_Port, A3_Pin);
		//	address = (address<<1)|HAL_GPIO_ReadPin(A2_GPIO_Port, A2_Pin);

		//	uint8_t _RW = HAL_GPIO_ReadPin(RW_GPIO_Port, RW_Pin);
	//if (_RW==1)
	//{
		//countR++;
	//}
	//else
	//{
		//countW++;
	//}




		//}


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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
