/**
 ******************************************************************************
 * @file    main.c
 * @author  Ac6
 * @version V1.0
 * @date    01-December-2013
 * @brief   Default main function.
 ******************************************************************************
 */

#include "stm32f7xx.h"
#include "stm32746g_discovery.h"
#include "SystemClockConfig.h"
#include "stm32746g_discovery_ts.h"
#include "stm32746g_discovery_lcd.h"
#include "dma2d.h"

typedef enum
{
  DRAWING_LAYER, COLOR_LAYER, FONT_LAYER, CLEAR_LAYER
} layer_state_t;

typedef struct
{
  uint8_t coordX;
  uint8_t coordY;
  uint8_t draw_color;
  uint8_t font_size;
}data_struct_t;

uint8_t touch_flag = 0;
layer_state_t layer_status = DRAWING_LAYER;
TS_StateTypeDef touch;
uint32_t rgb_code;
uint8_t font_size = 1;
uint32_t color_array[10] = { LCD_COLOR_BLACK, LCD_COLOR_RED, LCD_COLOR_GREEN, LCD_COLOR_BLUE, LCD_COLOR_GRAY,
LCD_COLOR_YELLOW, LCD_COLOR_BROWN, LCD_COLOR_MAGENTA, LCD_COLOR_CYAN, LCD_COLOR_ORANGE };
uint32_t drawing_color = LCD_COLOR_BLACK;
GPIO_InitTypeDef GPIOTxConfig;
UART_HandleTypeDef UartHandle;
data_struct_t data_package;

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIOTxConfig.Pin = GPIO_PIN_9;
  GPIOTxConfig.Mode = GPIO_MODE_AF_PP;
  GPIOTxConfig.Pull = GPIO_NOPULL;
  GPIOTxConfig.Speed = GPIO_SPEED_FAST;
  GPIOTxConfig.Alternate = GPIO_AF7_USART1;

  HAL_GPIO_Init(GPIOA, &GPIOTxConfig);
  __HAL_RCC_USART1_CLK_ENABLE();
  HAL_UART_Init(&UartHandle);

  UartHandle.Instance = USART1;
  UartHandle.Init.BaudRate = 115200;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits = UART_STOPBITS_1;
  UartHandle.Init.Parity = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode = UART_MODE_TX_RX;

  BSP_LCD_Init();
  BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
  BSP_LCD_LayerDefaultInit(1,
  LCD_FB_START_ADDRESS + (BSP_LCD_GetXSize() * BSP_LCD_GetYSize() * 4));
  BSP_LCD_SelectLayer(0);
  BSP_LCD_SetLayerVisible(0, ENABLE);
  BSP_LCD_SetLayerVisible(1, DISABLE);
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  BSP_TS_Init(480, 272);
  BSP_TS_ITConfig();
  rgb_code = 0000;

  DMA2D_DrawImage((uint32_t) paint_icon, 453, 0, 27, 27);
  DMA2D_DrawImage((uint32_t) font_icon, 453, 245, 27, 27);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t*) "Clear", LEFT_MODE);

  while (1) {
    if (touch_flag) {
      BSP_TS_GetState(&touch);
      touch_flag = 0;
      if (touch.touchX > 0 && touch.touchY > 0 && touch.touchX[0] <= 60 && touch.touchY[0] <= 20
          && layer_status == DRAWING_LAYER) {
        layer_status = CLEAR_LAYER;
      }
    }
    if (touch.touchX[0] >= 453 && touch.touchY[0] <= 27 && layer_status == DRAWING_LAYER) { // entering color layer
      BSP_LCD_SelectLayer(1);
      BSP_LCD_SetLayerVisible(1, ENABLE);
      BSP_LCD_SetLayerVisible(0, DISABLE);
      layer_status = COLOR_LAYER;
      BSP_LCD_Clear(LCD_COLOR_WHITE);
      int counter = 0;
      for (int i = 0; i < 2; i++) {
        for (int j = 1; j < 6; j++) {
          BSP_LCD_SetTextColor(color_array[counter]);
          BSP_LCD_FillRect(j * 40 + (j - 1) * 48, 112 - 34 + i * 48 + i * 34, 48, 48);
          counter++;
        }
      }
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_DisplayStringAt(100, 50, (uint8_t*) "Select draw color", LEFT_MODE);
      BSP_LCD_DisplayStringAt(0, 250, (uint8_t*) "Exit", LEFT_MODE);
    } else if (touch.touchX[0] >= 453 && touch.touchY[0] >= 245 && layer_status == DRAWING_LAYER) { // entering font layer
      BSP_LCD_SelectLayer(1);
      BSP_LCD_SetLayerVisible(1, ENABLE);
      BSP_LCD_SetLayerVisible(0, DISABLE);
      layer_status = FONT_LAYER;
      BSP_LCD_Clear(LCD_COLOR_WHITE);
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      for (int i = 1; i < 6; i++) {
        BSP_LCD_DrawRect(i * 40 + (i - 1) * 48, 112, 48, 48);
        BSP_LCD_DisplayChar(i * 50 + (i - 1) * 40, 130, 48 + i);
      }
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_DisplayStringAt(110, 80, (uint8_t*) "Select font size", LEFT_MODE);
      BSP_LCD_DisplayStringAt(0, 250, (uint8_t*) "Exit", LEFT_MODE);
    } else if (layer_status == FONT_LAYER) { // from font to drawing layer
      if (touch.touchX[0] >= 40 && touch.touchX[0] <= 88 && touch.touchY[0] >= 112 && touch.touchY[0] <= 156) {
        font_size = 1;
      } else if (touch.touchX[0] >= 128 && touch.touchX[0] <= 176 && touch.touchY[0] >= 116 && touch.touchY[0] <= 156) {
        font_size = 2;
      } else if (touch.touchX[0] >= 216 && touch.touchX[0] <= 264 && touch.touchY[0] >= 116 && touch.touchY[0] <= 156) {
        font_size = 3;
      } else if (touch.touchX[0] >= 304 && touch.touchX[0] <= 352 && touch.touchY[0] >= 116 && touch.touchY[0] <= 156) {
        font_size = 4;
      } else if (touch.touchX[0] >= 392 && touch.touchX[0] <= 440 && touch.touchY[0] >= 116 && touch.touchY[0] <= 156) {
        font_size = 5;
      } else if (touch.touchX[0] > 0 && touch.touchY[0] > 250 && touch.touchX[0] <= 60 && touch.touchY[0] <= 272
          && layer_status == FONT_LAYER) {
        BSP_LCD_SelectLayer(0);
        BSP_LCD_SetLayerVisible(0, ENABLE);
        BSP_LCD_SetLayerVisible(1, DISABLE);
        layer_status = DRAWING_LAYER;
      }
    } else if (layer_status == COLOR_LAYER) { // from color to drawing layer
      if (touch.touchX[0] >= 40 && touch.touchX[0] <= 88 && touch.touchY[0] >= 78 && touch.touchY[0] <= 126) {
        drawing_color = LCD_COLOR_BLACK;
      } else if (touch.touchX[0] >= 128 && touch.touchX[0] <= 176 && touch.touchY[0] >= 78 && touch.touchY[0] <= 126) {
        drawing_color = LCD_COLOR_RED;
      } else if (touch.touchX[0] >= 216 && touch.touchX[0] <= 264 && touch.touchY[0] >= 78 && touch.touchY[0] <= 126) {
        drawing_color = LCD_COLOR_GREEN;
      } else if (touch.touchX[0] >= 304 && touch.touchX[0] <= 352 && touch.touchY[0] >= 78 && touch.touchY[0] <= 126) {
        drawing_color = LCD_COLOR_BLUE;
      } else if (touch.touchX[0] >= 392 && touch.touchX[0] <= 440 && touch.touchY[0] >= 78 && touch.touchY[0] <= 126) {
        drawing_color = LCD_COLOR_GRAY;
      } else if (touch.touchX[0] >= 40 && touch.touchX[0] <= 88 && touch.touchY[0] >= 160 && touch.touchY[0] <= 208) {
        drawing_color = LCD_COLOR_YELLOW;
      } else if (touch.touchX[0] >= 128 && touch.touchX[0] <= 176 && touch.touchY[0] >= 160 && touch.touchY[0] <= 208) {
        drawing_color = LCD_COLOR_BROWN;
      } else if (touch.touchX[0] >= 216 && touch.touchX[0] <= 264 && touch.touchY[0] >= 160 && touch.touchY[0] <= 208) {
        drawing_color = LCD_COLOR_MAGENTA;
      } else if (touch.touchX[0] >= 304 && touch.touchX[0] <= 352 && touch.touchY[0] >= 160 && touch.touchY[0] <= 208) {
        drawing_color = LCD_COLOR_CYAN;
      } else if (touch.touchX[0] >= 392 && touch.touchX[0] <= 440 && touch.touchY[0] >= 160 && touch.touchY[0] <= 208) {
        drawing_color = LCD_COLOR_ORANGE;
      } else if (touch.touchX[0] > 0 && touch.touchY[0] > 250 && touch.touchX[0] <= 60 && touch.touchY[0] <= 272
          && layer_status == COLOR_LAYER) {
        BSP_LCD_SelectLayer(0);
        BSP_LCD_SetLayerVisible(0, ENABLE);
        BSP_LCD_SetLayerVisible(1, DISABLE);
        layer_status = DRAWING_LAYER;
      }
    } else if (touch.touchX > 0 && touch.touchY > 0 && touch.touchX[0] <= 60 && touch.touchY[0] <= 20
        && layer_status == CLEAR_LAYER) { // clearing drawing surface
      BSP_LCD_Clear(LCD_COLOR_WHITE);
      DMA2D_DrawImage((uint32_t) paint_icon, 453, 0, 27, 27);
      DMA2D_DrawImage((uint32_t) font_icon, 453, 245, 27, 27);
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_DisplayStringAt(0, 0, (uint8_t*) "Clear", LEFT_MODE);
      layer_status = DRAWING_LAYER;
    } else if (layer_status == DRAWING_LAYER
        && (touch.touchX[0] > 60 || (touch.touchY[0] > 20 && touch.touchY[0] < 240))) { // actual drawing
      data_package.coordX = touch.touchX[0];
      data_package.coordY = touch.touchY[0];
      data_package.draw_color = (uint8_t)drawing_color;
      data_package.font_size = font_size + 3;
      BSP_LCD_SetTextColor(drawing_color);
      BSP_LCD_FillRect(touch.touchX[0], touch.touchY[0], 2 + font_size, 2 + font_size);
      HAL_UART_Transmit(&UartHandle, &data_package.coordX, sizeof(data_package.coordX), 0xFFFF);
      HAL_UART_Transmit(&UartHandle, &data_package.coordY, sizeof(data_package.coordY), 0xFFFF);
      HAL_UART_Transmit(&UartHandle, &data_package.draw_color, sizeof(data_package.draw_color), 0xFFFF);
      HAL_UART_Transmit(&UartHandle, &data_package.font_size, sizeof(data_package.font_size), 0xFFFF);
    }
  }
}

void EXTI15_10_IRQHandler()
{
  HAL_GPIO_EXTI_IRQHandler(TS_INT_PIN);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == TS_INT_PIN) {
    touch_flag = 1;
  }
}
