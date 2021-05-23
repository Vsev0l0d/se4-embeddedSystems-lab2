#include "hal.h"
#define T 50
#define SIZE 8

int leds_num[] = {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_11, GPIO_PIN_12};
int sw_num[] = {GPIO_PIN_4, GPIO_PIN_8, GPIO_PIN_10, GPIO_PIN_12};
int i;

int get_SW() {
    return  8 * HAL_GPIO_ReadPin(GPIOE, sw_num[0]) +
            4 * HAL_GPIO_ReadPin(GPIOE, sw_num[1]) +
            2 * HAL_GPIO_ReadPin(GPIOE, sw_num[2]) +
            1 * HAL_GPIO_ReadPin(GPIOE, sw_num[3]);
}

void TIM6_IRQ_Handler() {
    for (int j = 0; j < SIZE; j++){
        if (j == i || j == i + 1 || j == SIZE - i - 1) HAL_GPIO_WritePin(GPIOD, leds_num[j], GPIO_PIN_SET);
        else HAL_GPIO_WritePin(GPIOD, leds_num[j], GPIO_PIN_RESET);
    }
    i++;
    if (i == SIZE) i = 0;
    WRITE_REG(TIM6_ARR, 500 + T * get_SW());
}

int umain() {
    i = 0;
    registerTIM6_IRQHandler(TIM6_IRQ_Handler);
    __enable_irq();
    WRITE_REG(TIM6_ARR, 500);
    WRITE_REG(TIM6_DIER, TIM_DIER_UIE);
    WRITE_REG(TIM6_PSC, 0);
    WRITE_REG(TIM6_CR1, TIM_CR1_CEN);
    return 0;
}