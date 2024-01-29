#include "stm32f4xx.h"
#include "delay.h"
#include"OLED.h"
#include "led.h"
#include "stdio.h"


// extern void led_out_gpio(uint8_t arg_int, int led_n);
// extern uint8_t led_in_gpio(int led_n);

#define LED0_GPIO       GPIOD
#define LED1_GPIO       GPIOD
#define LED2_GPIO       GPIOC

#define LED0_GPIO_PIN   GPIO_Pin_14
#define LED1_GPIO_PIN   GPIO_Pin_15
#define LED2_GPIO_PIN   GPIO_Pin_8


void led_gpio_init(void){
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void led_out_gpio(uint8_t arg_int, int led_n){
    switch (led_n){
        case 0:
            arg_int ? GPIO_SetBits(LED0_GPIO, LED0_GPIO_PIN): GPIO_ResetBits(LED0_GPIO, LED0_GPIO_PIN);
            break;
        case 1:
            arg_int ? GPIO_SetBits(LED1_GPIO, LED1_GPIO_PIN): GPIO_ResetBits(LED1_GPIO, LED1_GPIO_PIN);
            break;
        case 2:
            arg_int ? GPIO_SetBits(LED2_GPIO, LED2_GPIO_PIN): GPIO_ResetBits(LED2_GPIO, LED2_GPIO_PIN);
            break;
    }	
}

uint8_t led_in_gpio(int led_n){
    switch (led_n){
        case 0:
            return GPIO_ReadInputDataBit(LED0_GPIO, LED0_GPIO_PIN);
        case 1:
            return GPIO_ReadInputDataBit(LED1_GPIO, LED1_GPIO_PIN);
        case 2:
            return GPIO_ReadInputDataBit(LED2_GPIO, LED2_GPIO_PIN);
    }
    return 255;		
}


int main(void){

    uint8_t led_level   = 0;
    uint32_t led_levels = 0;

    OLED_Init();
    led_gpio_init();

    pLED_Class led = createLedClassWtihArgs(3);
    OLED_ShowNum(1,1,led->led_amount);
    
    delay_ms(1000);

    for(int i=0;i<3;++i){
        led->set_level(led, i, LED_HIGH_LEVEL);
        led->read_level(led,i, &led_level);
        OLED_ShowNum(2,1,led_level);
        delay_ms(1000);
        led->set_level(led, i, LED_LOW_LEVEL);
        led->read_level(led,i, &led_level);
        OLED_ShowNum(2,1,led_level);
        delay_ms(1000);
    }
    for(int i=0;i<0x08;++i){
        led->set_level_arr(led,i);
        led->read_level_arr(led, &led_levels);
        OLED_ShowBinNum(3,1,led_levels,8);
        delay_ms(1000);
    }
  
    while (1){
        
    } 
}
