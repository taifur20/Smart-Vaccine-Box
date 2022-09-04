/*
 * gps.h
 *
 * Created on: Aug 6, 2022
 * Author: Md. Khairul Alam
 */

/*
#ifndef NEO7_GPS_H_
#define NEO7_GPS_H_

#define DATA_RECEIVE_PIN GPIO_Pin_11
#define DATA_TRANSMIT_PIN GPIO_Pin_10

void GPSPORT_CFG(void);
void USART3_IRQHandler(void);

#endif
*/

#ifndef _GPS_H
#define _GPS_H

#include <stdio.h>
#include <string.h>
#include "debug.h"

#define RX_BUFFER_SIZE      1024


__packed typedef struct
{
    u8 num;             //satellite number
    u8 eledeg;          //elevation in degrees
    u16 azideg;         //azimuth in degrees to true
    u8 sn;              //SNR in dB
}nmea_slmsg;

//UTC
__packed typedef struct
{
        u16 year;
        u8 month;
        u8 date;
        u8 hour;
        u8 min;
        u8 sec;
}nmea_utc_time;

//NEMA 0183
__packed typedef struct
{
        u8 svnum;                               //satellites in view
        nmea_slmsg slmsg[12];
        nmea_utc_time utc;                      //UTC
        u32 latitude;                           //latitude * 100000
        u8 nshemi;                              //North or South
        u32 longitude;                          //longitude * 100000
        u8 ewhemi;                              //East or West
        u8 gpssta;                              //GPS quality indicator
        u8 posslnum;                            //Number of satellites in view, 00 - 12
        u8 possl[12];
        u8 fixmode;                             //mode
        u16 pdop;                               //PDOP in meters
        u16 hdop;                               //HDOP in meters
        u16 vdop;                               //VDOP in meters
        int altitude;                           //antenna altitude above/below mean-sea-level * 10
        u16 speed;                              //speed * 1000
}nmea_msg;

void GPSMSG_Analysis(nmea_msg *gpsx, u8 *buf);
void Send_NMEA_MSG(nmea_msg *gpsx);

void NMEA_GPRMC_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GPGSV_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GPGGA_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GPGSA_Analysis(nmea_msg *gpsx,u8 *buf);
void NMEA_GPVTG_Analysis(nmea_msg *gpsx,u8 *buf);

void Send_NMEA_GPRMC(nmea_msg *gpsx);
void Send_NMEA_GPGSV(nmea_msg *gpsx);
void Send_NMEA_GPGGA(nmea_msg *gpsx);
void Send_NMEA_GPGSA(nmea_msg *gpsx);
void Send_NMEA_GPVTG(nmea_msg *gpsx);
#endif /*_GPS_H*/
