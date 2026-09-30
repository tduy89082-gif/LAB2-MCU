/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : LAB 2 - EXERCISE 2
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;


/* USER CODE BEGIN PV */


uint8_t led_buffer[4] = {1, 2, 3, 0};


uint8_t current_led = 0;


uint16_t counter_500ms = 0;
uint16_t counter_1s = 0;



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


/* USER CODE BEGIN 0 */



void display7SEG(uint8_t num)
{
    uint8_t data;

    if (num > 9)
    {
        num = 0;
    }

    data = seg7_code[num];


    HAL_GPIO_WritePin(
        GPIOB,
        GPIO_PIN_0,
        (data & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );


    HAL_GPIO_WritePin(
        GPIOB,
        GPIO_PIN_1,
        (data & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );


    HAL_GPIO_WritePin(
        GPIOB,
        GPIO_PIN_2,
        (data & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );


    HAL_GPIO_WritePin(
        GPIOB,
        GPIO_PIN_3,
        (data & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );


    HAL_GPIO_WritePin(
        GPIOB,
        GPIO_PIN_4,
        (data & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );


    HAL_GPIO_WritePin(
        GPIOB,
        GPIO_PIN_5,
        (data & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );


    /* SEG6 -> PB6 */
    HAL_GPIO_WritePin(
        GPIOB,
        GPIO_PIN_6,
        (data & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );
}


void disableAllLED(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
}



void enableLED(uint8_t index)
{
    disableAllLED();


    switch (index)
    {
        case 0:
            HAL_GPIO_WritePin(
                GPIOA,
                GPIO_PIN_6,
                GPIO_PIN_RESET
            );
            break;


        case 1:
            HAL_GPIO_WritePin(
                GPIOA,
                GPIO_PIN_7,
                GPIO_PIN_RESET
            );
            break;


        case 2:
            HAL_GPIO_WritePin(
                GPIOA,
                GPIO_PIN_8,
                GPIO_PIN_RESET
            );
            break;


        case 3:
            HAL_GPIO_WritePin(
                GPIOA,
                GPIO_PIN_9,
                GPIO_PIN_RESET
            );
            break;


        default:
            break;
    }
}


/* USER CODE END 0 */


/* ============================================================
 * MAIN
 * ============================================================ */
int main(void)
{
    /* Reset of all peripherals, Initializes the Flash interface
       and the Systick. */
    HAL_Init();


    /* Configure System Clock */
    SystemClock_Config();


    /* Initialize GPIO */
    MX_GPIO_Init();


    /* Initialize TIM2 */
    MX_TIM2_Init();




    disableAllLED();



    HAL_GPIO_WritePin(
        GPIOA,
        GPIO_PIN_4,
        GPIO_PIN_SET
    );


    display7SEG(led_buffer[0]);


    enableLED(0);



    HAL_TIM_Base_Start_IT(&htim2);


    /* Infinite loop */
    while (1)
    {

    }
}



void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};


    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;

    RCC_OscInitStruct.HSIState = RCC_HSI_ON;

    RCC_OscInitStruct.HSICalibrationValue =
        RCC_HSICALIBRATION_DEFAULT;

    RCC_OscInitStruct.PLL.PLLState =
        RCC_PLL_NONE;


    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }


    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK |
        RCC_CLOCKTYPE_SYSCLK |
        RCC_CLOCKTYPE_PCLK1 |
        RCC_CLOCKTYPE_PCLK2;


    RCC_ClkInitStruct.SYSCLKSource =
        RCC_SYSCLKSOURCE_HSI;


    RCC_ClkInitStruct.AHBCLKDivider =
        RCC_SYSCLK_DIV1;


    RCC_ClkInitStruct.APB1CLKDivider =
        RCC_HCLK_DIV1;


    RCC_ClkInitStruct.APB2CLKDivider =
        RCC_HCLK_DIV1;


    if (HAL_RCC_ClockConfig(
            &RCC_ClkInitStruct,
            FLASH_LATENCY_0) != HAL_OK)
    {
        Error_Handler();
    }
}



static void MX_TIM2_Init(void)
{
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};


    htim2.Instance = TIM2;


    htim2.Init.Prescaler = 7999;


    htim2.Init.CounterMode =
        TIM_COUNTERMODE_UP;


    htim2.Init.Period = 9;


    htim2.Init.ClockDivision =
        TIM_CLOCKDIVISION_DIV1;


    htim2.Init.AutoReloadPreload =
        TIM_AUTORELOAD_PRELOAD_DISABLE;


    if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
    {
        Error_Handler();
    }


    sClockSourceConfig.ClockSource =
        TIM_CLOCKSOURCE_INTERNAL;


    if (HAL_TIM_ConfigClockSource(
            &htim2,
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



static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};


    /* Enable GPIO clocks */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();


    /*
     * ========================================================
     * GPIOA
     * ========================================================
     */



    HAL_GPIO_WritePin(
        GPIOA,
        GPIO_PIN_4 |
        GPIO_PIN_6 |
        GPIO_PIN_7 |
        GPIO_PIN_8 |
        GPIO_PIN_9,
        GPIO_PIN_SET
    );



    GPIO_InitStruct.Pin =
        GPIO_PIN_4 |
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


    HAL_GPIO_Init(
        GPIOA,
        &GPIO_InitStruct
    );


    /*
     * ========================================================
     * GPIOB
     * ========================================================
     */


    HAL_GPIO_WritePin(
        GPIOB,
        GPIO_PIN_0 |
        GPIO_PIN_1 |
        GPIO_PIN_2 |
        GPIO_PIN_3 |
        GPIO_PIN_4 |
        GPIO_PIN_5 |
        GPIO_PIN_6,
        GPIO_PIN_SET
    );


    /*
     * PB0-PB6 = SEG0-SEG6
     */
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


    HAL_GPIO_Init(
        GPIOB,
        &GPIO_InitStruct
    );
}


/* ============================================================
 * TIMER INTERRUPT CALLBACK
 *
 * TIM2 interrupt = 10 ms
 * ============================================================ */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {

        counter_500ms++;


        if (counter_500ms >= 50)
        {
            counter_500ms = 0;



            current_led++;


            if (current_led >= 4)
            {
                current_led = 0;
            }



            disableAllLED();



            display7SEG(
                led_buffer[current_led]
            );



            enableLED(current_led);
        }



        counter_1s++;


        if (counter_1s >= 100)
        {
            counter_1s = 0;



            HAL_GPIO_TogglePin(
                GPIOA,
                GPIO_PIN_4
            );
        }
    }
}


/* ============================================================
 * ERROR HANDLER
 * ============================================================ */
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
    /* User can add implementation to report file name
       and line number */
}

#endif
