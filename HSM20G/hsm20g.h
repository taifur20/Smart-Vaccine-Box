/*
 * hsm20g.h
 *
 *  Created on: Aug 5, 2022
 *  Author: Md. Khairul Alam
 */

#ifndef HSM20G_HSM20G_H_
#define HSM20G_HSM20G_H_

#define TEMP_PIN GPIO_Pin_1
#define HUMID_PIN GPIO_Pin_2

void  ADC_Function_Init(void);
u16 Get_TEMP_ConversionVal(s16 val);
u16 Get_HUMID_ConversionVal(s16 val);
//void get_temp_value(void);
//void get_humid_value(void);
s32 get_temp_value(void);
s32 get_humid_value(void);

#endif /* HSM20G_HSM20G_H_ */
