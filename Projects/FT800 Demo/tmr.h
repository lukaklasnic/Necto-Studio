#ifndef TMR_H
#define TMR_H

#include "stdint.h"

#define PERIPH_BASE         0x40000000UL
#define APB1PERIPH_BASE     PERIPH_BASE
#define AHB1PERIPH_BASE     (PERIPH_BASE + 0x00020000UL)

#define RCC_BASE            (AHB1PERIPH_BASE + 0x3800UL)
#define RCC_APB1ENR         (*(volatile uint32_t *)(RCC_BASE + 0x40UL))
#define RCC_APB1ENR_TIM6EN  (1UL << 4) 

typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
} TIM_TypeDef;

#define TIM6_BASE           (APB1PERIPH_BASE + 0x1000UL)
#define TIM6                ((TIM_TypeDef *)TIM6_BASE)

#define TIM_CR1_CEN         (1UL << 0)
#define TIM_DIER_UIE        (1UL << 0)
#define TIM_SR_UIF          (1UL << 0)

// --- DEFINICIJE ZA NVIC (Prekide) ---

#define SCS_BASE            0xE000E000UL
#define NVIC_BASE           (SCS_BASE + 0x0100UL)

typedef struct {
    volatile uint32_t ISER[8];
    uint32_t RESERVED0[24];
    volatile uint32_t ICER[8];
    uint32_t RESERVED1[24];
    volatile uint32_t ISPR[8];
    uint32_t RESERVED2[24];
    volatile uint32_t ICPR[8];
    uint32_t RESERVED3[24];
    volatile uint32_t IABR[8];
    uint32_t RESERVED4[56];
    volatile uint8_t  IP[240];
} NVIC_Type;

#define NVIC                ((NVIC_Type *)NVIC_BASE)

#define TIM6_DAC_IRQn       54

static inline void NVIC_EnableIRQ(int IRQn);
static inline void NVIC_SetPriority(int IRQn, uint32_t priority);
void timer6_init(uint32_t frequency_hz);

#endif 