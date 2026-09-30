/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Lab 2 - Exercise 1
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*
 * Ma 7 doan Common Anode
 *
 * PB0 -> SEG0
 * PB1 -> SEG1
 * PB2 -> SEG2
 * PB3 -> SEG3
 * PB4 -> SEG4
 * PB5 -> SEG5
 * PB6 -> SEG6
 *
 * Bit = 0 -> sang
 * Bit = 1 -> tat
 */

uint8_t seg7[] =
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


/*
 * Bien dem Timer
 *
 * TIM2 ngat moi 10 ms
 *
 * 50 lan = 500 ms
 * 100 lan = 1000 ms
 */
int counter = 0;


/*
 * LED dang duoc hien thi
 *
 * 0 -> LED 7 doan thu nhat
 * 1 -> LED 7 doan thu hai
 */
int current_led = 0;


/*
 * Hien thi so len LED 7 doan
 */
void display7SEG(int num)
{
    uint8_t data = seg7[num];

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


/*
 * Tat ca 2 LED 7 doan
 *
 * PA6 -> EN0
 * PA7 -> EN1
 *
 * Do dung PNP:
 * 0 -> ON
 * 1 -> OFF
 */
void clearLed(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
}


/*
 * Chon LED 7 doan
 *
 * index = 0 -> LED thu nhat
 * index = 1 -> LED thu hai
 */
void enablePin(int index)
{
    clearLed();

    if (index == 0)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
    }
    else if (index == 1)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
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
  MX_TIM2_Init();

  /* USER CODE BEGIN 2 */

  /*
   * Ban dau hien thi so 1
   * tren LED 7 doan thu nhat
   */
  display7SEG(1);
  enablePin(0);

  /*
   * Khoi dong TIM2 voi interrupt
   */
  HAL_TIM_Base_Start_IT(&htim2);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

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

  /** Initializes the RCC Oscillators
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
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
  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */

  htim2.Instance = TIM2;

  /*
   * Clock = 8 MHz
   *
   * 8,000,000 / (7999 + 1)
   * = 1000 Hz
   *
   * 1000 / (9 + 1)
   * = 100 Hz
   *
   * => interrupt moi 10 ms
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

  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*
   * PA5 -> LED RED
   * PA6 -> EN0
   * PA7 -> EN1
   *
   * PNP:
   * SET   -> OFF
   * RESET -> ON
   */
  HAL_GPIO_WritePin(GPIOA,
                    GPIO_PIN_5 |
                    GPIO_PIN_6 |
                    GPIO_PIN_7,
                    GPIO_PIN_SET);

  /*
   * PB0 -> PB6
   */
  HAL_GPIO_WritePin(GPIOB,
                    GPIO_PIN_0 |
                    GPIO_PIN_1 |
                    GPIO_PIN_2 |
                    GPIO_PIN_3 |
                    GPIO_PIN_4 |
                    GPIO_PIN_5 |
                    GPIO_PIN_6,
                    GPIO_PIN_SET);

  /*
   * PA5 PA6 PA7
   */
  GPIO_InitStruct.Pin =
      GPIO_PIN_5 |
      GPIO_PIN_6 |
      GPIO_PIN_7;

  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


  /*
   * PB0 PB1 PB2 PB3 PB4 PB5 PB6
   */
  GPIO_InitStruct.Pin =
      GPIO_PIN_0 |
      GPIO_PIN_1 |
      GPIO_PIN_2 |
      GPIO_PIN_3 |
      GPIO_PIN_4 |
      GPIO_PIN_5 |
      GPIO_PIN_6;

  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}


/* USER CODE BEGIN 4 */


/**
  * @brief Timer 2 interrupt callback
  *
  * TIM2 ngat moi 10 ms
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        counter++;

        /*
         * 50 x 10 ms = 500 ms
         *
         * Sau 500 ms doi sang LED thu hai
         */
        if (counter == 50)
        {
            display7SEG(2);
            enablePin(1);
        }

        /*
         * 100 x 10 ms = 1000 ms
         *
         * Sau 1 giay quay lai LED thu nhat
         */
        if (counter >= 100)
        {
            counter = 0;

            display7SEG(1);
            enablePin(0);

            /*
             * D1 doi trang thai moi 1 giay
             */
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
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
  /* USER CODE BEGIN Error_Handler_Debug */

  __disable_irq();

  while (1)
  {
  }

  /* USER CODE END Error_Handler_Debug */
}


#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */

  /* User can add his own implementation to report the file name and line number */

  /* USER CODE END 6 */
}

#endif /* USE_FULL_ASSERT */
