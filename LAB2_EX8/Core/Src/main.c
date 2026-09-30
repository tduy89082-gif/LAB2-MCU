/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Exercise 8 - Software Timer + LED Scanning
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* =========================
   SOFT TIMER
   TIM2 interrupt = 10 ms
   ========================= */
volatile int timer1_counter = 0;
volatile int timer1_flag = 0;

volatile int timer2_counter = 0;
volatile int timer2_flag = 0;

volatile int timer3_counter = 0;
volatile int timer3_flag = 0;

const int TIMER_CYCLE = 10;

/* =========================
   7 SEGMENT
   ========================= */
int index_led = 0;

uint8_t led_buffer[4] = {1, 5, 0, 8};

/* Common Anode 7SEG
   PB0 -> a
   PB1 -> b
   PB2 -> c
   PB3 -> d
   PB4 -> e
   PB5 -> f
   PB6 -> g
*/
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

/* =========================
   CLOCK
   ========================= */
int hour = 15;
int minute = 8;
int second = 50;

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

void setTimer1(int duration);
void setTimer2(int duration);
void setTimer3(int duration);

void timerRun(void);

/* USER CODE END PFP */


/* USER CODE BEGIN 0 */

/* =========================
   DISPLAY 7 SEGMENT
   ========================= */
void display7SEG(int num)
{
    uint8_t code;

    if (num < 0 || num > 9)
    {
        code = 0xFF;
    }
    else
    {
        code = seg7_code[num];
    }

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0,
                      (code & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,
                      (code & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2,
                      (code & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3,
                      (code & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4,
                      (code & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,
                      (code & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6,
                      (code & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}


/* =========================
   TURN OFF ALL 4 LED 7SEG
   PNP ENABLE ACTIVE LOW
   => HIGH = OFF
   ========================= */
void clearLed(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
}


/* =========================
   UPDATE 1 LED 7SEG
   index 0 -> LED1 -> PA6
   index 1 -> LED2 -> PA7
   index 2 -> LED3 -> PA8
   index 3 -> LED4 -> PA9
   ========================= */
void update7SEG(int index)
{
    clearLed();

    display7SEG(led_buffer[index]);

    switch (index)
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

        default:
            clearLed();
            break;
    }
}


/* =========================
   UPDATE CLOCK BUFFER
   15:08
   => 1 5 0 8
   ========================= */
void updateClockBuffer(void)
{
    led_buffer[0] = hour / 10;
    led_buffer[1] = hour % 10;
    led_buffer[2] = minute / 10;
    led_buffer[3] = minute % 10;
}


/* =========================
   SOFTWARE TIMER 1
   LED RED + DOT
   ========================= */
void setTimer1(int duration)
{
    timer1_counter = duration / TIMER_CYCLE;
    timer1_flag = 0;
}


/* =========================
   SOFTWARE TIMER 2
   7SEG SCANNING
   ========================= */
void setTimer2(int duration)
{
    timer2_counter = duration / TIMER_CYCLE;
    timer2_flag = 0;
}


/* =========================
   SOFTWARE TIMER 3
   CLOCK
   ========================= */
void setTimer3(int duration)
{
    timer3_counter = duration / TIMER_CYCLE;
    timer3_flag = 0;
}


/* =========================
   RUN ALL SOFTWARE TIMERS
   Called every 10 ms
   ========================= */
void timerRun(void)
{
    if (timer1_counter > 0)
    {
        timer1_counter--;

        if (timer1_counter == 0)
        {
            timer1_flag = 1;
        }
    }

    if (timer2_counter > 0)
    {
        timer2_counter--;

        if (timer2_counter == 0)
        {
            timer2_flag = 1;
        }
    }

    if (timer3_counter > 0)
    {
        timer3_counter--;

        if (timer3_counter == 0)
        {
            timer3_flag = 1;
        }
    }
}


/* =========================
   TIM2 INTERRUPT CALLBACK
   IMPORTANT:
   ONLY RUN SOFTWARE TIMER
   ========================= */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        timerRun();
    }
}

/* USER CODE END 0 */


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* Reset of all peripherals, Initializes the Flash interface and SysTick. */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_TIM2_Init();

    /* USER CODE BEGIN 2 */

    /* Start TIM2 interrupt */
    HAL_TIM_Base_Start_IT(&htim2);

    /* Initial clock */
    updateClockBuffer();

    /* Start software timers */
    setTimer1(1000);   // LED + DOT every 1 second
    setTimer2(50);     // Scan 7SEG every 50 ms
    setTimer3(1000);   // Clock update every 1 second

    /* USER CODE END 2 */


    /* Infinite loop */
    while (1)
    {
        /* =====================================
           TIMER 1:
           BLINK LED-RED + DOT EVERY 1 SECOND
           ===================================== */
        if (timer1_flag == 1)
        {
            timer1_flag = 0;

            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);

            setTimer1(1000);
        }


        /* =====================================
           TIMER 2:
           SCAN 4 LED 7SEG
           ===================================== */
        if (timer2_flag == 1)
        {
            timer2_flag = 0;

            update7SEG(index_led);

            index_led++;

            if (index_led >= 4)
            {
                index_led = 0;
            }

            setTimer2(50);
        }


        /* =====================================
           TIMER 3:
           UPDATE CLOCK EVERY 1 SECOND
           ===================================== */
        if (timer3_flag == 1)
        {
            timer3_flag = 0;

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

    /* 8 MHz / (7999 + 1) = 1 kHz
       1 kHz / (9 + 1) = 100 Hz
       => interrupt every 10 ms
    */
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

    if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
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

    /* Initial state:
       PA4 DOT      = OFF
       PA5 LED RED  = OFF
       PA6-PA9 EN   = OFF
    */

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

    /* GPIOA PA4 -> PA9 */
    GPIO_InitStruct.Pin = GPIO_PIN_4 |
                          GPIO_PIN_5 |
                          GPIO_PIN_6 |
                          GPIO_PIN_7 |
                          GPIO_PIN_8 |
                          GPIO_PIN_9;

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


    /* GPIOB PB0 -> PB6 */
    GPIO_InitStruct.Pin = GPIO_PIN_0 |
                          GPIO_PIN_1 |
                          GPIO_PIN_2 |
                          GPIO_PIN_3 |
                          GPIO_PIN_4 |
                          GPIO_PIN_5 |
                          GPIO_PIN_6;

    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
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
