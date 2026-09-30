#include "main.h"

TIM_HandleTypeDef htim2;


/* ============================================================
 * BÀI 4 - TIMER INTERRUPT AND LED SCANNING
 *
 * PA4  -> DOT
 * PA5  -> LED-RED
 * PA6  -> EN0 -> LED1
 * PA7  -> EN1 -> LED2
 * PA8  -> EN2 -> LED3
 * PA9  -> EN3 -> LED4
 *
 * PB0 -> SEG0
 * PB1 -> SEG1
 * PB2 -> SEG2
 * PB3 -> SEG3
 * PB4 -> SEG4
 * PB5 -> SEG5
 * PB6 -> SEG6
 *
 * TIM2 interrupt = 10 ms
 *
 * Bài 4:
 * - update7SEG mỗi 250 ms
 * - 4 LED hoàn thành 1 vòng trong 1 giây
 * - LED-RED + DOT toggle mỗi 1 giây
 * ============================================================ */


/* ============================================================
 * 7 SEG CODE - COMMON ANODE
 * ============================================================ */

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


/* ============================================================
 * BÀI 4
 * ============================================================ */

#define MAX_LED 4

int index_led = 0;

/*
 * Giá trị hiển thị:
 *
 * LED1 -> 1
 * LED2 -> 2
 * LED3 -> 3
 * LED4 -> 4
 */
int led_buffer[4] = {1, 2, 3, 4};


/*
 * TIM2 interrupt = 10 ms
 *
 * 100 x 10 ms = 1000 ms = 1 s
 */
int counter = 100;


/* ============================================================
 * FUNCTION PROTOTYPE
 * ============================================================ */

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);

void display7SEG(int num);
void clearLed(void);
void enablePin(int index);
void update7SEG(int index);


/* ============================================================
 * DISPLAY 7 SEGMENT
 * ============================================================ */

void display7SEG(int num)
{
    uint8_t data;

    if (num < 0 || num > 9)
    {
        num = 0;
    }

    data = seg7_code[num];


    /* SEG0 -> PB0 */
    HAL_GPIO_WritePin(
        GPIOB,
        GPIO_PIN_0,
        (data & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );


    /* SEG1 -> PB1 */
    HAL_GPIO_WritePin(
        GPIOB,
        GPIO_PIN_1,
        (data & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );


    /* SEG2 -> PB2 */
    HAL_GPIO_WritePin(
        GPIOB,
        GPIO_PIN_2,
        (data & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );


    /* SEG3 -> PB3 */
    HAL_GPIO_WritePin(
        GPIOB,
        GPIO_PIN_3,
        (data & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );


    /* SEG4 -> PB4 */
    HAL_GPIO_WritePin(
        GPIOB,
        GPIO_PIN_4,
        (data & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );


    /* SEG5 -> PB5 */
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


/* ============================================================
 * TẮT 4 LED 7 ĐOẠN
 *
 * PNP:
 * HIGH = OFF
 * LOW  = ON
 * ============================================================ */

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


/* ============================================================
 * ENABLE LED
 *
 * index 0 -> PA6 -> LED1
 * index 1 -> PA7 -> LED2
 * index 2 -> PA8 -> LED3
 * index 3 -> PA9 -> LED4
 * ============================================================ */

void enablePin(int index)
{
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


/* ============================================================
 * UPDATE 7 SEG
 * ============================================================ */

void update7SEG(int index)
{
    /* Tắt tất cả LED trước */
    clearLed();


    switch (index)
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


        default:

            break;
    }
}


/* ============================================================
 * MAIN
 * ============================================================ */

int main(void)
{
    HAL_Init();

    SystemClock_Config();

    MX_GPIO_Init();

    MX_TIM2_Init();


    /* Tắt tất cả LED 7 đoạn */
    clearLed();


    /* LED-RED OFF */
    HAL_GPIO_WritePin(
        GPIOA,
        GPIO_PIN_5,
        GPIO_PIN_SET
    );


    /* DOT OFF */
    HAL_GPIO_WritePin(
        GPIOA,
        GPIO_PIN_4,
        GPIO_PIN_SET
    );


    /*
     * Hiển thị LED1 = 1 ngay từ đầu
     */
    update7SEG(index_led);


    /*
     * Bắt đầu TIM2 interrupt
     */
    HAL_TIM_Base_Start_IT(&htim2);


    while (1)
    {
        /*
         * Không cần code ở đây.
         * Tất cả xử lý nằm trong Timer Interrupt.
         */
    }
}


/* ============================================================
 * SYSTEM CLOCK
 *
 * HSI = 8 MHz
 * ============================================================ */

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};


    RCC_OscInitStruct.OscillatorType =
        RCC_OSCILLATORTYPE_HSI;

    RCC_OscInitStruct.HSIState =
        RCC_HSI_ON;

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


/* ============================================================
 * TIM2 INIT
 *
 * Clock = 8 MHz
 * Prescaler = 7999
 * Period = 9
 *
 * => Interrupt = 10 ms
 * ============================================================ */

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


/* ============================================================
 * GPIO INIT
 * ============================================================ */

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};


    /* Enable GPIO clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();


    /* ========================================================
     * GPIOA
     * ======================================================== */

    /*
     * PA4 = DOT
     * PA5 = LED-RED
     * PA6 = EN0
     * PA7 = EN1
     * PA8 = EN2
     * PA9 = EN3
     *
     * HIGH = OFF
     */

    HAL_GPIO_WritePin(
        GPIOA,
        GPIO_PIN_4 |
        GPIO_PIN_5 |
        GPIO_PIN_6 |
        GPIO_PIN_7 |
        GPIO_PIN_8 |
        GPIO_PIN_9,
        GPIO_PIN_SET
    );


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


    HAL_GPIO_Init(
        GPIOA,
        &GPIO_InitStruct
    );


    /* ========================================================
     * GPIOB
     * ======================================================== */

    /*
     * PB0 -> SEG0
     * PB1 -> SEG1
     * PB2 -> SEG2
     * PB3 -> SEG3
     * PB4 -> SEG4
     * PB5 -> SEG5
     * PB6 -> SEG6
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
 * TIMER INTERRUPT - BÀI 4
 *
 * TIM2 interrupt = 10 ms
 *
 * counter = 100
 *
 * Mỗi 25 lần interrupt:
 *
 * 25 x 10 ms = 250 ms
 *
 * => đổi LED 7 SEG mỗi 250 ms
 *
 * 4 LED:
 *
 * LED1 -> 250 ms
 * LED2 -> 250 ms
 * LED3 -> 250 ms
 * LED4 -> 250 ms
 *
 * Tổng = 1000 ms = 1 giây
 *
 * => Frequency của 4 LED = 1 Hz
 *
 * LED-RED + DOT:
 * toggle mỗi 1000 ms
 * ============================================================ */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        if (counter > 0)
        {
            counter--;


            /* =================================================
             * SWITCH 4 LED 7 SEG EVERY 250 ms
             *
             * counter % 25 == 0
             *
             * 25 x 10 ms = 250 ms
             * ================================================= */

            if (counter % 25 == 0)
            {
                update7SEG(index_led);

                index_led++;


                if (index_led >= MAX_LED)
                {
                    index_led = 0;
                }
            }


            /* =================================================
             * LED-RED + DOT BLINK EVERY 1 SECOND
             * ================================================= */

            if (counter == 0)
            {
                /* LED-RED */
                HAL_GPIO_TogglePin(
                    GPIOA,
                    GPIO_PIN_5
                );


                /* DOT */
                HAL_GPIO_TogglePin(
                    GPIOA,
                    GPIO_PIN_4
                );


                /* Reset 1 second counter */
                counter = 100;
            }
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
}

#endif
