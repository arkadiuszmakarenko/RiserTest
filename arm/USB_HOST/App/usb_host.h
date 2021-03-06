/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usb_host.h
  * @version        : v1.0_Cube
  * @brief          : Header for usb_host.c file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_HOST__H__
#define __USB_HOST__H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"
#include "usbh_hid_mouse.h"

/* USER CODE BEGIN INCLUDE */
 typedef struct _HID_USBDev
 {
   uint8_t *gamepad1;
   uint8_t *gamepad2;
   HID_KEYBD_Info_TypeDef *keyboard;
   USBH_HandleTypeDef* keyboardusbhost;
   HID_MOUSE_Info_TypeDef *mouse;

 }
 HID_USBDevicesTypeDef;

/* USER CODE END INCLUDE */

/** @addtogroup USBH_OTG_DRIVER
  * @{
  */

/** @defgroup USBH_HOST USBH_HOST
  * @brief Host file for Usb otg low level driver.
  * @{
  */

/** @defgroup USBH_HOST_Exported_Variables USBH_HOST_Exported_Variables
  * @brief Public variables.
  * @{
  */

/**
  * @}
  */

/** Status of the application. */
typedef enum {
  APPLICATION_IDLE = 0,
  APPLICATION_START,
  APPLICATION_READY,
  APPLICATION_DISCONNECT
}ApplicationTypeDef;

/** @defgroup USBH_HOST_Exported_FunctionsPrototype USBH_HOST_Exported_FunctionsPrototype
  * @brief Declaration of public functions for Usb host.
  * @{
  */

/* Exported functions -------------------------------------------------------*/

/** @brief USB Host initialization function. */
void MX_USB_HOST_Init(void);

void MX_USB_HOST_Process(void);
HID_USBDevicesTypeDef* USBH_HID_GetUSBDev();
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USB_HOST__H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
