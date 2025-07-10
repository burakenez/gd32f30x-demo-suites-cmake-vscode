/*!
    \file  netconf.h
    \brief the header file of netconf 
*/

/*
    Copyright (C) 2017 GigaDevice

    2017-07-28, V1.0.0, demo for GD32F30x
*/

#ifndef NETCONF_H
#define NETCONF_H
#include "main.h"

/* function declarations */
/* initializes the LwIP stack */
void lwip_stack_init(void);
/* dhcp_task */
void dhcp_task(void * pvParameters);
/* netif status callback function */
void lwip_netif_status_callback(struct netif *netif);

#endif /* NETCONF_H */
