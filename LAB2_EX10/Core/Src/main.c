/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);


const int MAX_LED_MATRIX = 8;


int index_led = 0;

int index_led_matrix = 0;
int matrix_shift_flag = 0;
uint8_t shift = 0;

uint8_t matrix_buffer[8] = {
    0xC7,
    0x93,
    0x39,
    0x01,
    0x01,
    0x39,
    0x39,
    0x39
};


int led_buffer[4] = {1, 2, 3, 4};

int hour = 15;
int minute = 8;
int second = 57;


int timer1_counter = 0;
int timer2_counter = 0;
int timer3_counter = 0;

int timer1_flag = 0;
int timer2_flag = 0;
int timer3_flag = 0;


void setTimer1(int duration)
{
    timer1_counter = duration / 10;
    timer1_flag = 0;
}

void setTimer2(int duration)
{
    timer2_counter = duration / 10;
    timer2_flag = 0;
}

void setTimer3(int duration)
{
    timer3_counter = duration / 10;
    timer3_flag = 0;
}

void timerRun(void)
{
    if (timer1_counter > 0)
    {
        timer1_counter--;

        if (timer1_counter <= 0)
        {
            timer1_flag = 1;
        }
    }

    if (timer2_counter > 0)
    {
        timer2_counter--;

        if (timer2_counter <= 0)
        {
            timer2_flag = 1;
        }
    }

    if (timer3_counter > 0)
    {
        timer3_counter--;

        if (timer3_counter <= 0)
        {
            timer3_flag = 1;
        }
    }
}


void clearLed(void)
{
    HAL_GPIO_WritePin(
        GPIOA,
        GPIO_PIN_6 |
        GPIO_PIN_7 |
        GPIO_PIN_8 |
        GPIO_PIN_9,
        GPIO_PIN_SET
    );
}

void clearPin(int index)
{
    switch(index)
    {
        case 0:
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
            break;

        case 1:
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
            break;

        case 2:
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
            break;

        case 3:
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
            break;
    }
}

void enablePin(int index)
{
    switch(index)
    {
        case 0:
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
            break;

        case 1:
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
            break;

        case 2:
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
            break;

        case 3:
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
            break;
    }
}

void display7SEG(int num)
{
    char segNumber[10] = {
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

    for (int i = 0; i < 7; i++)
    {
        HAL_GPIO_WritePin(
            GPIOB,
            (GPIO_PIN_0 << i),
            (segNumber[num] >> i) & 0x01
        );
    }
}

void clearEnable(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
}

void update7SEG(int index)
{
    clearLed();
    clearEnable();

    switch(index)
    {
        case 0:
            display7SEG(led_buffer[0]);
            enablePin(0);
            break;

        case 1:
            display7SEG(led_buffer[1]);
            enablePin(1);
            break;

        case 2:
            display7SEG(led_buffer[2]);
            enablePin(2);
            break;

        case 3:
            display7SEG(led_buffer[3]);
            enablePin(3);
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


void setCol(uint8_t val)
{
    HAL_GPIO_WritePin(
        GPIOA,
        GPIO_PIN_2,
        ((val >> 7) & 0x01)
    );

    HAL_GPIO_WritePin(
        GPIOA,
        GPIO_PIN_3,
        ((val >> 6) & 0x01)
    );

    HAL_GPIO_WritePin(
        GPIOA,
        GPIO_PIN_10,
        ((val >> 5) & 0x01)
    );

    HAL_GPIO_WritePin(
        GPIOA,
        GPIO_PIN_11,
        ((val >> 4) & 0x01)
    );

    HAL_GPIO_WritePin(
        GPIOA,
        GPIO_PIN_12,
        ((val >> 3) & 0x01)
    );

    HAL_GPIO_WritePin(
        GPIOA,
        GPIO_PIN_13,
        ((val >> 2) & 0x01)
    );

    HAL_GPIO_WritePin(
        GPIOA,
        GPIO_PIN_14,
        ((val >> 1) & 0x01)
    );

    HAL_GPIO_WritePin(
        GPIOA,
        GPIO_PIN_15,
        ((val >> 0) & 0x01)
    );
}

/* ---------------------------------------------------------
 * SET ALL ROWS HIGH
 * --------------------------------------------------------- */
void setMatrix(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8,  GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,  GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
}

/* ---------------------------------------------------------
 * UPDATE LED MATRIX
 * --------------------------------------------------------- */
void updateLEDMatrix(uint8_t index, uint8_t shift)
{
    setMatrix();

    uint8_t matrix_buffer_shift =
        (matrix_buffer[index] << shift) |
        (matrix_buffer[index] >> (8 - shift));

    switch(index)
    {
        case 0:
            setCol(matrix_buffer_shift);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
            break;

        case 1:
            setCol(matrix_buffer_shift);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
            break;

        case 2:
            setCol(matrix_buffer_shift);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
            break;

        case 3:
            setCol(matrix_buffer_shift);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
            break;

        case 4:
            setCol(matrix_buffer_shift);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
            break;

        case 5:
            setCol(matrix_buffer_shift);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
            break;

        case 6:
            setCol(matrix_buffer_shift);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
            break;

        case 7:
            setCol(matrix_buffer_shift);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
            break;

        default:
            break;
    }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

    HAL_Init();

    SystemClock_Config();

    __HAL_RCC_AFIO_CLK_ENABLE();
    __HAL_AFIO_REMAP_SWJ_DISABLE();

    MX_GPIO_Init();
    MX_TIM2_Init();

    HAL_TIM_Base_Start_IT(&htim2);

    updateClockBuffer();

    setTimer1(1000);
    setTimer2(10);
    setTimer3(1000);

    while (1)
    {
        /* =================================================
         * LED RED + DOT BLINK EVERY 1 SECOND
         * ================================================= */
        if (timer1_flag == 1)
        {
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);   // LED RED
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);   // DOT

            setTimer1(1000);
        }


        if (timer2_flag == 1)
        {
            /* 7 SEG */
            update7SEG(index_led);

            index_led++;

            if (index_led >= 4)
            {
                index_led = 0;
            }

            /* LED MATRIX */
            updateLEDMatrix(index_led_matrix, shift);

            index_led_matrix++;

            if (index_led_matrix >= 8)
            {
                index_led_matrix = 0;

                shift++;

                if (shift >= 8)
                {
                    shift = 0;
                }
            }

            setTimer2(10);
        }


        if (timer3_flag == 1)
        {
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

            setTimer3(1000);
        }
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

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
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

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    htim2.Instance = TIM2;

    htim2.Init.Prescaler = 7999;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 9;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
    {
        Error_Handler();
    }

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;

    if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

    if (HAL_TIMEx_MasterConfigSynchronization(
            &htim2,
            &sMasterConfig
        ) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();



    HAL_GPIO_WritePin(
        GPIOA,
        GPIO_PIN_2  |
        GPIO_PIN_3  |
        GPIO_PIN_4  |
        GPIO_PIN_5  |
        GPIO_PIN_6  |
        GPIO_PIN_7  |
        GPIO_PIN_8  |
        GPIO_PIN_9  |
        GPIO_PIN_10 |
        GPIO_PIN_11 |
        GPIO_PIN_12 |
        GPIO_PIN_13 |
        GPIO_PIN_14 |
        GPIO_PIN_15,
        GPIO_PIN_RESET
    );

    GPIO_InitStruct.Pin =
        GPIO_PIN_2  |
        GPIO_PIN_3  |
        GPIO_PIN_4  |
        GPIO_PIN_5  |
        GPIO_PIN_6  |
        GPIO_PIN_7  |
        GPIO_PIN_8  |
        GPIO_PIN_9  |
        GPIO_PIN_10 |
        GPIO_PIN_11 |
        GPIO_PIN_12 |
        GPIO_PIN_13 |
        GPIO_PIN_14 |
        GPIO_PIN_15;

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);



    HAL_GPIO_WritePin(
        GPIOB,
        GPIO_PIN_0  |
        GPIO_PIN_1  |
        GPIO_PIN_2  |
        GPIO_PIN_3  |
        GPIO_PIN_4  |
        GPIO_PIN_5  |
        GPIO_PIN_6  |
        GPIO_PIN_8  |
        GPIO_PIN_9  |
        GPIO_PIN_10 |
        GPIO_PIN_11 |
        GPIO_PIN_12 |
        GPIO_PIN_13 |
        GPIO_PIN_14 |
        GPIO_PIN_15,
        GPIO_PIN_RESET
    );

    GPIO_InitStruct.Pin =
        GPIO_PIN_0  |
        GPIO_PIN_1  |
        GPIO_PIN_2  |
        GPIO_PIN_3  |
        GPIO_PIN_4  |
        GPIO_PIN_5  |
        GPIO_PIN_6  |
        GPIO_PIN_8  |
        GPIO_PIN_9  |
        GPIO_PIN_10 |
        GPIO_PIN_11 |
        GPIO_PIN_12 |
        GPIO_PIN_13 |
        GPIO_PIN_14 |
        GPIO_PIN_15;

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* =========================================================
 * TIM2 CALLBACK
 * ========================================================= */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        timerRun();
    }
}

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
#endif
