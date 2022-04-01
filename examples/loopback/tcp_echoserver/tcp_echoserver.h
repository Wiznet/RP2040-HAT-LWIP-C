/**
 * Copyright (c) 2022 WIZnet Co.,Ltd
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _TCP_ECHOSERVER_H_
#define _TCP_ECHOSERVER_H_

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
extern struct netif g_netif;
static struct tcp_pcb *tcp_echoserver_pcb;

/* ECHO protocol states */
enum tcp_echoserver_states
{
    ES_NONE = 0,
    ES_ACCEPTED,
    ES_RECEIVED,
    ES_CLOSING
};

/* structure for maintaing connection infos to be passed as argument
   to LwIP callbacks*/
struct tcp_echoserver_struct
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
void tcp_echoserver_init(void);
err_t tcp_echoserver_accept(void *arg, struct tcp_pcb *newpcb, err_t err);
err_t tcp_echoserver_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
void tcp_echoserver_error(void *arg, err_t err);
err_t tcp_echoserver_poll(void *arg, struct tcp_pcb *tpcb);
err_t tcp_echoserver_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
void tcp_echoserver_send(struct tcp_pcb *tpcb, struct tcp_echoserver_struct *es);
void tcp_echoserver_connection_close(struct tcp_pcb *tpcb, struct tcp_echoserver_struct *es);

#endif /* _TCP_ECHOSERVER_H_ */
