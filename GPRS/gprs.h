/*
 * gprs.h
 *
 *  Created on: Aug 6, 2022
 *  Author: Md. Khairul Alam
 */

#ifndef GPRS_GPRS_H_
#define GPRS_GPRS_H_

void send_data(char *msg, int first_val, int second_val);
void send_apikey(char *msg, int temp, int humid);
void send_command(u8 *buf);
void send_test_sms(void);
void make_test_call(void);
void temperature_high_alart(void);
void humidity_high_alart(void);

#endif /* GPRS_GPRS_H_ */
