/*!
    \file  main.h
    \brief the header file of main 
*/

/*
    Copyright (C) 2017 GigaDevice

    2017-07-28, V1.0.0, demo for GD32F30x
*/

#ifndef MAIN_H
#define MAIN_H

#include "gd32f30x.h"
#include "stdint.h"
#include "FreeRTOS.h"
#include "task.h"
#include "gd32f30x_enet_eval.h"


//#define USE_DHCP       /* enable DHCP, if disabled static address is used */

/* MAC address: BOARD_MAC_ADDR0:BOARD_MAC_ADDR1:BOARD_MAC_ADDR2:BOARD_MAC_ADDR3:BOARD_MAC_ADDR4:BOARD_MAC_ADDR5 */
#define BOARD_MAC_ADDR0   2
#define BOARD_MAC_ADDR1   0xA
#define BOARD_MAC_ADDR2   0xF
#define BOARD_MAC_ADDR3   0xE
#define BOARD_MAC_ADDR4   0xD
#define BOARD_MAC_ADDR5   6
 
/* static IP address: BOARD_IP_ADDR0.BOARD_IP_ADDR1.BOARD_IP_ADDR2.BOARD_IP_ADDR3 */
#define BOARD_IP_ADDR0   10
#define BOARD_IP_ADDR1   50
#define BOARD_IP_ADDR2   3
#define BOARD_IP_ADDR3   210

/* remote station IP address: IP_S_ADDR0.IP_S_ADDR1.IP_S_ADDR2.IP_S_ADDR3 */
#define IP_S_ADDR0   10
#define IP_S_ADDR1   50
#define IP_S_ADDR2   3
#define IP_S_ADDR3   140

/* net mask */
#define BOARD_NETMASK_ADDR0   255
#define BOARD_NETMASK_ADDR1   255
#define BOARD_NETMASK_ADDR2   255
#define BOARD_NETMASK_ADDR3   0

/* gateway address */
#define BOARD_GW_ADDR0   10
#define BOARD_GW_ADDR1   50
#define BOARD_GW_ADDR2   3
#define BOARD_GW_ADDR3   1

/* MII and RMII mode selection */
#define RMII_MODE  // user have to provide the 50 MHz clock by soldering a 50 MHz oscillator
//#define MII_MODE

/* clock the PHY from external 25MHz crystal (only for MII mode) */
#ifdef  MII_MODE
#define PHY_CLOCK_MCO
#endif

#endif /* MAIN_H */
