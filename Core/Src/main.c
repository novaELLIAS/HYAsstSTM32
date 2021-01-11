
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention designed for STM32F411CEU6
  ******************************************************************************
  */

#define SerialDebug
#define SerialGPSdebug
//#define SerialDebugFloatTest

#include "main.h"
#include <stdio.h>
#include "LED_Functions/LED_OUTPUT.h"
#include "GPS_Decoder/GPSdecode.h"
#include "MPU6050/Accident_Alert.h"

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_USART1_UART_Init(void);

// Serial output for Debug @ HUART1
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE {
	HAL_UART_Transmit((UART_HandleTypeDef *)&huart1, (uint8_t*)&ch, 1, 0xFFFF);
	return ch;
}


// GPS decoder

#define GPS_Delay_Time 1000
uint8_t       gps_init;
uint8_t       gps_uart[5000];
nmea_slmsg    NMEAslmsg;
nmea_utc_time NMEAutctime;
nmea_msg      NMEAmsg;
gps_data      NMEAdata;

void GPS_decode (void) {

	HAL_UART_Receive(&huart2, gps_uart, sizeof(gps_uart), GPS_Delay_Time);

	#ifdef SerialGPSdebug
		//printf("USART data:\r\n%s\r\n", gps_uart);
	#endif

	NMEA_GPGGA_Analysis (&NMEAmsg, (uint8_t*) gps_uart);

	if (!NMEAmsg.gpssta) {
		LED_GPSRFS_ON();
		#ifdef SerialDebug
			printf("** GPS NO SIGNAL **\r\n");
		#endif
	} else {
		LED_GPSRFS_ON();
		NMEA_GPRMC_Analysis (&NMEAmsg, (uint8_t*) gps_uart);
		NMEA_GPGSA_Analysis (&NMEAmsg, (uint8_t*) gps_uart);
		NMEA_GPVTG_Analysis (&NMEAmsg, (uint8_t*) gps_uart);
		NMEA_GPRMC_Analysis (&NMEAmsg, (uint8_t*) gps_uart);

		NMEA_GPS_DATA_PHRASE(&NMEAmsg, &NMEAdata);

		#ifdef SerialDebug
			printf("\r\n** GPS Serial Debug **\r\n");
			printf("GPS status: %s, PDOT: %f\r\n", NMEAmsg.gpssta^2? "3D":"2D", NMEAdata.pdop);
			printf("UTC time: %02d:%02d:%02d\r\n", NMEAmsg.utc.hour, NMEAmsg.utc.min, NMEAmsg.utc.sec);
			printf("Lat: %.6f, Log: %.6f, Spd: %.6f\r\n", NMEAdata.latitude, NMEAdata.longitude, NMEAdata.speed);
		#endif

		LED_GPSRFS_OFF();
	}
}

#undef GPS_Delay_Time

// main

signed main(void) {

	HAL_Init();

	SystemClock_Config();

	MX_GPIO_Init();
	MX_I2C1_Init();
	MX_USART2_UART_Init();
	MX_USART6_UART_Init();
	MX_USART1_UART_Init();

	accidentMonitorSetup();

	LED_OUTPUT_INIT();
	LED_PC13_INIT();

	LED_OUTPUT_TEST();

	HAL_UART_Receive_IT(&huart2, &gps_init, 1);

	#ifdef SerialDebugFloatTest
		printf("initialization success.\r\n");
		register float nnnnn = 114.514;
		printf("\r\nFLOAT TEST\r\n%f\r\n", nnnnn);
		nnnnn = 1919.810;
		printf("%f\r\n\r\n", nnnnn);
	#endif

	while (1) {
	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	*/
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/** Initializes the RCC Oscillators according to the specified parameters
	* in the RCC_OscInitTypeDef structure.
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
															|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
}

/**
	* @brief I2C1 Initialization Function
	* @param None
	* @retval None
	*/
static void MX_I2C1_Init(void) {

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 400000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
	* @brief USART1 Initialization Function
	* @param None
	* @retval None
	*/
static void MX_USART1_UART_Init(void) {

	/* USER CODE BEGIN USART1_Init 0 */

	/* USER CODE END USART1_Init 0 */

	/* USER CODE BEGIN USART1_Init 1 */

	/* USER CODE END USART1_Init 1 */
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN USART1_Init 2 */

	/* USER CODE END USART1_Init 2 */

}

/**
	* @brief USART2 Initialization Function
	* @param None
	* @retval None
	*/
static void MX_USART2_UART_Init(void) {

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 9600;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
	* @brief USART6 Initialization Function
	* @param None
	* @retval None
	*/
static void MX_USART6_UART_Init(void) {

	/* USER CODE BEGIN USART6_Init 0 */

	/* USER CODE END USART6_Init 0 */

	/* USER CODE BEGIN USART6_Init 1 */

	/* USER CODE END USART6_Init 1 */
	huart6.Instance = USART6;
	huart6.Init.BaudRate = 115200;
	huart6.Init.WordLength = UART_WORDLENGTH_8B;
	huart6.Init.StopBits = UART_STOPBITS_1;
	huart6.Init.Parity = UART_PARITY_NONE;
	huart6.Init.Mode = UART_MODE_TX_RX;
	huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart6.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart6) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART6_Init 2 */

	/* USER CODE END USART6_Init 2 */

}

/**
	* @brief GPIO Initialization Function
	* @param None
	* @retval None
	*/
static void MX_GPIO_Init(void) {

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
	* @brief	This function is executed in case of error occurrence.
	* @retval None
	*/
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
		LED_PC13_BLINK(1000);
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef	USE_FULL_ASSERT
/**
	* @brief	Reports the name of the source file and the source line number
	*				 where the assert_param error has occurred.
	* @param	file: pointer to the source file name
	* @param	line: assert_param error line source number
	* @retval None
	*/
void assert_failed(uint8_t *file, uint32_t line) {
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	   ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
