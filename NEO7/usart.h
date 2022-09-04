/*
 * usart.h
 *
 *  Created on: Aug 6, 2022
 *  Author: Md. Khairul Alam
 */

#ifndef NEO7_USART_H_
#define NEO7_USART_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdio.h>

void usart_configuration(void);
void timer_configuration(u16 arr, u16 psc);
void USART3_IRQHandler(void);
void TIM7_IRQHandler(void);
void TIM7_Set(u8 mode);


#ifdef __cplusplus
}
#endif

#endif /* NEO7_USART_H_ */
