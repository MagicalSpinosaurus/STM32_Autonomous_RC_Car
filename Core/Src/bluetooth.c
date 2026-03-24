#include "bluetooth.h"
#include "usart.h"

static volatile uint8_t flags = 0;
static volatile uint8_t modes = 0;

static volatile uint8_t rxData = 0;

static void Bluetooth_ClearErrors(void)
{
  __HAL_UART_CLEAR_OREFLAG(&huart1);
  __HAL_UART_CLEAR_FEFLAG(&huart1);
  __HAL_UART_CLEAR_NEFLAG(&huart1);
  __HAL_UART_CLEAR_PEFLAG(&huart1);
}

void Bluetooth_Init()
{
  flags = 0;
  modes = 0;
}

void Bluetooth_Ctrl(uint8_t data)
{
  switch(data)
  {
    case 'A' :
      flags = 1;
      break;
    case 'B' :
      flags = 2;
      break;
    case 'C' :
      flags = 3;
      break;
    case 'D' :
      flags = 4;
      break;
    case 'L' :
      flags = 5;
      break;
    case 'R' :
      flags = 6;
      break;
    case '0' :
      flags = 0;
      break;

    case 'U' :
      modes = 1;
      flags = 0;
      break;
    case 'M' :
      modes = 0;
      flags = 0;
      break;

    default :
      break;
  }
}

uint8_t Bluetooth_GetFlag()
{
  return flags;
}

uint8_t Bluetooth_GetMode()
{
  return modes;
}

void Bluetooth_Start()
{
  Bluetooth_ClearErrors();
}

void Bluetooth_Poll()
{
  if (huart1.Instance == NULL)
  {
    return;
  }

  if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_ORE) ||
      __HAL_UART_GET_FLAG(&huart1, UART_FLAG_FE)  ||
      __HAL_UART_GET_FLAG(&huart1, UART_FLAG_NE)  ||
      __HAL_UART_GET_FLAG(&huart1, UART_FLAG_PE))
  {
    Bluetooth_ClearErrors();
  }

  while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) != RESET)
  {
    rxData = (uint8_t)(huart1.Instance->DR & 0x00FFU);
    Bluetooth_Ctrl(rxData);
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance != USART1)
  {
    return;
  }

  Bluetooth_Ctrl(rxData);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance != USART1)
  {
    return;
  }

  Bluetooth_ClearErrors();
}
