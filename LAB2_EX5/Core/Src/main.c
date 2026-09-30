/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : LAB 2 - EX5 - Digital Clock
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;


volatile int hour   = 15;
volatile int minute = 8;
volatile int second = 50;


volatile int led_buffer[4] = {1, 5, 0, 8};


const uint8_t seg7_code[10] =
{
    0xC0,   // 0
    0xF9,   // 1
    0xA4,   // 2
    0xB0,   // 3
    0x99,   // 4
    0x92,   // 5
    0x82,   // 6
    0xF8,   // 7
    0x80,   // 8
    0x90    // 9
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);

/* USER CODE BEGIN PFP */

void display7SEG(int num);
void clearLed(void);
void update7SEG(int index);
void updateClockBuffer(void);

/* USER CODE END PFP */


/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */



void display7SEG(int num)
{
    uint8_t data;

    if (num < 0 || num > 9)
    {
        return;
    }

    data = seg7_code[num];

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0,
                      (data & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,
                      (data & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2,
                      (data & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3,
                      (data & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4,
                      (data & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,
                      (data & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6,
                      (data & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}



void clearLed(void)
{
    HAL_GPIO_WritePin(GPIOA,
                      GPIO_PIN_6 |
                      GPIO_PIN_7 |
                      GPIO_PIN_8 |
                      GPIO_PIN_9,
                      GPIO_PIN_SET);
}


/* =========================================================
 * QUET LED
 *
 * index 0 -> PA6 -> LED1 -> so 1
 * index 1 -> PA7 -> LED2 -> so 5
 * index 2 -> PA8 -> LED3 -> so 0
 * index 3 -> PA9 -> LED4 -> so 8
 * ========================================================= */
void update7SEG(int index)
{
    if (index < 0 || index > 3)
    {
        return;
    }

    clearLed();

    display7SEG(led_buffer[index]);

    switch(index)
    {
        case 0:
            /* LED1 */
            HAL_GPIO_WritePin(GPIOA,
                              GPIO_PIN_6,
                              GPIO_PIN_RESET);
            break;

        case 1:
            /* LED2 */
            HAL_GPIO_WritePin(GPIOA,
                              GPIO_PIN_7,
                              GPIO_PIN_RESET);
            break;

        case 2:
            /* LED3 */
            HAL_GPIO_WritePin(GPIOA,
                              GPIO_PIN_8,
                              GPIO_PIN_RESET);
            break;

        case 3:
            /* LED4 */
            HAL_GPIO_WritePin(GPIOA,
                              GPIO_PIN_9,
                              GPIO_PIN_RESET);
            break;

        default:
            break;
    }
}



void updateClockBuffer(void)
{
    led_buffer[0] = hour / 10;
    led_buffer[1] = hour % 10;
    led_buffer[2] = minute / 10;
    led_buffer[3] = minute % 10;
}


/* USER CODE END 0 */


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* MCU Configuration--------------------------------------------------------*/

    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_TIM2_Init();

    /* USER CODE BEGIN 2 */

    updateClockBuffer();

    HAL_TIM_Base_Start_IT(&htim2);

    /* USER CODE END 2 */


    /* Infinite loop */
    while (1)
    {
        HAL_Delay(1000);

        second++;

        if (second >= 60)
        {
            second = 0;
            minute++;
        }

        if (minute >= 60)
        {
            minute = 0;
            hour++;
        }

        if (hour >= 24)
        {
            hour = 0;
        }

        updateClockBuffer();
    }
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* HSI = 8 MHz */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue =
        RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK |
        RCC_CLOCKTYPE_SYSCLK |
        RCC_CLOCKTYPE_PCLK1 |
        RCC_CLOCKTYPE_PCLK2;

    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct,
                            FLASH_LATENCY_0) != HAL_OK)
    {
        Error_Handler();
    }
}


/**
  * @brief TIM2 Initialization Function
  * @retval None
  */
static void MX_TIM2_Init(void)
{
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    htim2.Instance = TIM2;

    /* =====================================================
     * HSI = 8 MHz
     *
     * 8,000,000 / (7999 + 1) = 1000 Hz
     *
     * 1000 / (9 + 1) = 100 Hz
     *
     * => 10 ms / interrupt
     * ===================================================== */
    htim2.Init.Prescaler = 7999;

    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;

    htim2.Init.Period = 9;

    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    htim2.Init.AutoReloadPreload =
        TIM_AUTORELOAD_PRELOAD_DISABLE;

    if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
    {
        Error_Handler();
    }

    sClockSourceConfig.ClockSource =
        TIM_CLOCKSOURCE_INTERNAL;

    if (HAL_TIM_ConfigClockSource(&htim2,
                                  &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }

    sMasterConfig.MasterOutputTrigger =
        TIM_TRGO_RESET;

    sMasterConfig.MasterSlaveMode =
        TIM_MASTERSLAVEMODE_DISABLE;

    if (HAL_TIMEx_MasterConfigSynchronization(
            &htim2,
            &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
}


/**
  * @brief GPIO Initialization Function
  * @retval None
  */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable GPIO clocks */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();




    HAL_GPIO_WritePin(GPIOA,
                      GPIO_PIN_4 |
                      GPIO_PIN_5 |
                      GPIO_PIN_6 |
                      GPIO_PIN_7 |
                      GPIO_PIN_8 |
                      GPIO_PIN_9,
                      GPIO_PIN_SET);




    HAL_GPIO_WritePin(GPIOB,
                      GPIO_PIN_0 |
                      GPIO_PIN_1 |
                      GPIO_PIN_2 |
                      GPIO_PIN_3 |
                      GPIO_PIN_4 |
                      GPIO_PIN_5 |
                      GPIO_PIN_6,
                      GPIO_PIN_SET);




    GPIO_InitStruct.Pin =
        GPIO_PIN_4 |
        GPIO_PIN_5 |
        GPIO_PIN_6 |
        GPIO_PIN_7 |
        GPIO_PIN_8 |
        GPIO_PIN_9;

    GPIO_InitStruct.Mode =
        GPIO_MODE_OUTPUT_PP;

    GPIO_InitStruct.Pull =
        GPIO_NOPULL;

    GPIO_InitStruct.Speed =
        GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);




    GPIO_InitStruct.Pin =
        GPIO_PIN_0 |
        GPIO_PIN_1 |
        GPIO_PIN_2 |
        GPIO_PIN_3 |
        GPIO_PIN_4 |
        GPIO_PIN_5 |
        GPIO_PIN_6;

    GPIO_InitStruct.Mode =
        GPIO_MODE_OUTPUT_PP;

    GPIO_InitStruct.Pull =
        GPIO_NOPULL;

    GPIO_InitStruct.Speed =
        GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}


/* USER CODE BEGIN 4 */


/**
  * @brief TIM2 Period Elapsed Callback
  *

  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    static uint8_t index_led = 0;

    if (htim->Instance == TIM2)
    {
        update7SEG(index_led);

        index_led++;

        if (index_led >= 4)
        {
            index_led = 0;
        }
    }
}


/* USER CODE END 4 */


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    __disable_irq();

    while (1)
    {
    }
}


#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{
}

#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
