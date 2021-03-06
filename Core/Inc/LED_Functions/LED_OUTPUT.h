void LED_PC13_INIT ();
void LED_PC13_BLINK (register int);
void LED_PC13_BREATHE (register int);

void LED_OUTPUT_INIT ();

#define LED_TEST_Toggle()   HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13)
#define LED_TEST_OFF()      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET)
#define LED_TEST_ON()       HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET)

#define LED_GPSRFS_Toggle() HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7)
#define LED_GPSRFS_OFF()    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET)
#define LED_GPSRFS_ON()     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET)

#define LED_DATUPD_Toggle() HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6)
#define LED_DATUPD_OFF()    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET)
#define LED_DATUPD_ON()     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET)

#define LED_ALERT_Toggle()  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5)
#define LED_ALERT_OFF()     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET)
#define LED_ALERT_ON()      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET)

void LED_OUTPUT_TEST ();
