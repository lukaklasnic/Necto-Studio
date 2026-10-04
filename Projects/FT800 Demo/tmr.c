#include "tmr.h"


static inline void NVIC_EnableIRQ(int IRQn) {
    NVIC->ISER[((uint32_t)(int)IRQn) >> 5] = (1UL << (((uint32_t)(int)IRQn) & 0x1FUL));
}

static inline void NVIC_SetPriority(int IRQn, uint32_t priority) {
    if (IRQn >= 0) {
        NVIC->IP[((uint32_t)(int)IRQn)] = (uint8_t)((priority << (8 - 3)) & (uint32_t)0xFF);
    }
}



void my_periodic_task(void)
{

}

void timer6_init(uint32_t frequency_hz)
{
    RCC_APB1ENR |= RCC_APB1ENR_TIM6EN;

    uint32_t prescaler = 540 - 1; 
    uint32_t arr_value = (100000 / frequency_hz) - 1;

    TIM6->PSC = (uint16_t)prescaler;
    TIM6->ARR = (uint16_t)arr_value;

    TIM6->DIER |= TIM_DIER_UIE;

    NVIC_SetPriority(TIM6_DAC_IRQn, 2);
    NVIC_EnableIRQ(TIM6_DAC_IRQn);

    TIM6->CR1 |= TIM_CR1_CEN;
}

void TIM6_DAC_IRQHandler(void)
{
    if (TIM6->SR & TIM_SR_UIF)
    {
        TIM6->SR &= ~TIM_SR_UIF;
        my_periodic_task();
    }
}