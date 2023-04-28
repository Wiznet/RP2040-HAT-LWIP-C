/**
 * Copyright (c) 2023 WIZnet Co.,Ltd
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _TCP_ECHOCLIENT_H_
#define _TCP_ECHOCLIENT_H_

#include "lwip/tcp.h"

/**
 * ----------------------------------------------------------------------------------------------------
 * Includes
 * ----------------------------------------------------------------------------------------------------
 */

/**
 * ----------------------------------------------------------------------------------------------------
 * Macros
 * ----------------------------------------------------------------------------------------------------
 */
/* Port */
#define PORT_LOOPBACK 5001

/**
 * ----------------------------------------------------------------------------------------------------
 * Variables
 * ----------------------------------------------------------------------------------------------------
 */
static struct tcp_pcb *tcp_echoclient_pcb;
extern struct netif g_netif;
static ip_addr_t server_addr;

/* ECHO protocol states */
enum tcp_echoclient_states
{
    EC_NONE = 0,
    EC_CONNECTED,
    EC_RECEIVED,
    EC_CLOSING,
};

/* structure for maintaing connection infos to be passed as argument
   to LwIP callbacks*/
struct tcp_echoclient_struct
{
    u8_t state; /* current connection state */
    u8_t retries;
    struct tcp_pcb *pcb; /* pointer on the current tcp_pcb */
    struct pbuf *p;      /* pointer on the received/to be transmitted pbuf */
};

/**
 * ----------------------------------------------------------------------------------------------------
 * Functions
 * ----------------------------------------------------------------------------------------------------
 */
void tcp_echoclient_init(void);
static err_t tcp_echoclient_connected(void *arg, struct tcp_pcb *pcb_new, err_t err);
static err_t tcp_echoclient_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
static err_t tcp_echoclient_received(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static err_t tcp_echoclient_poll(void *arg, struct tcp_pcb *tpcb);
static void tcp_echoclient_send(struct tcp_pcb *tpcb, struct tcp_echoclient_struct *ec);
static void tcp_echoclient_error(void *arg, err_t err);
static void tcp_echoclient_connection_close(struct tcp_pcb *tpcb, struct tcp_echoclient_struct *ec);

#endif /* _TCP_ECHOCLIENT_H_ */
