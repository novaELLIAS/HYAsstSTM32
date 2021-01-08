/**
  ******************************************************************************
  * @file           : LED_OUTPUT.c
  * @author         : ElliasKiriStuart
  * @brief          : LED_CONTROL_FUNCTIONS
  ******************************************************************************
  * @attention
  *
  * Designed for STM32F411CEU6
  * site: https://brynhild.online/
  *
  ******************************************************************************
  */

#include "LED_OUTPUT.h"
#include "main.h"

inline void LED_PC13_INIT () {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOC_CLK_ENABLE();
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

inline void LED_PC13_BLINK (register int delayTime) {
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    HAL_Delay(delayTime);
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    HAL_Delay(delayTime);
}

inline void LED_PC13_BREATHE (register int len) {
	for (register int i=1; i<=len; ++ i) {
		for (register int j=1; j<=i; ++ j) {
			LED_TEST_ON(); HAL_Delay(4);
		} for (register int j=1; j<=len-i; ++ j) {
			LED_TEST_OFF(); HAL_Delay(4);
		}
	} for (register int i=1; i<=len; ++ i) {
		for (register int j=1; j<=len-i; ++ j) {
			LED_TEST_ON(); HAL_Delay(4);
		} for (register int j=1; j<=i; ++ j) {
			LED_TEST_OFF(); HAL_Delay(4);
		}
	}
}

inline void LED_OUTPUT_INIT () {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

	GPIO_InitStruct.Pin = GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}


