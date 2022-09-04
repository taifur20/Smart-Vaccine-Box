/*
 * usart.c
 *
 *  Created on: Aug 6, 2022
 *  Author: Md. Khairul Alam
 */
#include "debug.h"
#include "usart.h"
#include "gps.h"

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

void USART3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM7_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

extern u8 GPS_BUF[RX_BUFFER_SIZE];
extern vu16 GPSMSG_FLAG;
extern vu16 GPSMSG_INDEX;
//extern vu8 GPS_DEBUG;

void usart_configuration(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef  NVIC_InitStructure = {0};

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    /* USART3 TX-->PB.10  RX-->PB.11 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART3, &USART_InitStructure);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //USART_IT_RXNE - Receive Data register not empty interrupt.

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART3, ENABLE);
}

void timer_configuration(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
    NVIC_InitTypeDef  NVIC_InitStructure = {0};
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM7, ENABLE);
}


void TIM7_Set(u8 mode)
{
    if (mode)
    {
        TIM_SetCounter(TIM7, 0);
        TIM_Cmd(TIM7, ENABLE);
    }
    else
    {
        TIM_Cmd(TIM7, DISABLE);
    }
}


void USART3_IRQHandler(void)
{
    u8 receive_data;
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        receive_data = USART_ReceiveData(USART3);
        //printf("%c", receive_data);
        if(GPSMSG_INDEX<RX_BUFFER_SIZE)
        {
            TIM_SetCounter(TIM7, 0);
            if(GPSMSG_INDEX==0 && GPSMSG_FLAG == 0) TIM7_Set(1);
            GPS_BUF[GPSMSG_INDEX++]=receive_data;
        }else {
            GPSMSG_FLAG = 1;
        }
    }
    //printf("%s", GPS_BUF);
}

void TIM7_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
    {
        GPSMSG_FLAG = 1;
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
        TIM7_Set(0);
    }
}

#if 1
#pragma import(__use_no_semihosting)
struct __FILE
{
    int handle;
};
FILE __stdout;

_sys_exit(int x)
{
    //x;// = x;
}
#endif

PUTCHAR_PROTOTYPE
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART */
    USART_SendData(USART1,(u8)ch);

    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

    return ch;
}
