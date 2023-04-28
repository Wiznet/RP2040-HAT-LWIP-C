/**
 * Copyright (c) 2023 WIZnet Co.,Ltd
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * ----------------------------------------------------------------------------------------------------
 * Includes
 * ----------------------------------------------------------------------------------------------------
 */
#include <stdio.h>

#include "tcp_echoclient.h"
/**
 * ----------------------------------------------------------------------------------------------------
 * Macros
 * ----------------------------------------------------------------------------------------------------
 */

/**
 * ----------------------------------------------------------------------------------------------------
 * Variables
 * ----------------------------------------------------------------------------------------------------
 */
static u8_t dest_ip[4] = {192, 168, 11, 74};
static u16_t dest_port = 5000;

/**
 * ----------------------------------------------------------------------------------------------------
 * Functions
 * ----------------------------------------------------------------------------------------------------
 */
/**
 * @brief  Initializes the tcp echo client
 * @param  None
 * @retval None
 */
void tcp_echoclient_init(void)
{
    /* create new tcp pcb */
    tcp_echoclient_pcb = tcp_new();

    if (tcp_echoclient_pcb != NULL)
    {
        err_t err;

        /* destination ip address set (ECHO protocol) */
        IP4_ADDR(&server_addr, dest_ip[0], dest_ip[1], dest_ip[2], dest_ip[3]);
        /* connect echo_pcb to port 5000 (ECHO protocol) */
        err = tcp_connect(tcp_echoclient_pcb, &server_addr, dest_port, tcp_echoclient_connected);
        if (err == ERR_OK)
        {
            printf("[LWIP TCP Client] connect to : %d.%d.%d.%d", dest_ip[0], dest_ip[1], dest_ip[2], dest_ip[3]);
        }
    }
    else
    {
        memp_free(MEMP_TCP_PCB, tcp_echoclient_pcb);
        tcp_echoclient_pcb = NULL;
    }
}

/**
 * @brief  This function is the implementation of tcp_connected LwIP callback
 * @param  arg: not used
 * @param  newpcb: pointer on tcp_pcb struct for the newly created tcp connection
 * @param  err: not used
 * @retval err_t: error status
 */
err_t tcp_echoclient_connected(void *arg, struct tcp_pcb *newpcb, err_t err)
{
    err_t ret_err;
    struct tcp_echoclient_struct *ec;

    LWIP_UNUSED_ARG(arg);
    LWIP_UNUSED_ARG(err);

    /* error when connect to the server */
    if (err != ERR_OK) 
        return err;
    
    /* set priority for the client pcb */
    tcp_setprio(newpcb, TCP_PRIO_NORMAL);

    /* allocate structure es to maintain tcp connection informations */
    ec = (struct tcp_echoclient_struct *)mem_malloc(sizeof(struct tcp_echoclient_struct));
    if (ec != NULL)
    {
        ec->state = EC_CONNECTED;
        ec->pcb = newpcb;
        ec->retries = 0;
        ec->p = NULL;

        /* pass newly allocated es structure as argument to newpcb */
        tcp_arg (newpcb, ec);

        /* initialize lwip tcp_err callback function for newpcb  */
        tcp_err (newpcb, tcp_echoclient_error);

        /* initialize lwip tcp_sent callback function for newpcb  */
        tcp_sent(newpcb, tcp_echoclient_sent);

        /* initialize lwip tcp_recv callback function for newpcb  */
        tcp_recv(newpcb, tcp_echoclient_received); 

        /* initialize lwip tcp_poll callback function for newpcb */
        tcp_poll(newpcb, tcp_echoclient_poll, 0); 

        ret_err = ERR_OK;
    }
    else
    {
        /*  close tcp connection */
        tcp_echoclient_connection_close(newpcb, ec);
        /* return memory error */
        ret_err = ERR_MEM;
    }
    
    return ret_err;
}

/**
 * @brief  This function is the implementation for tcp_recv LwIP callback
 * @param  arg: pointer on a argument for the tcp_pcb connection
 * @param  tpcb: pointer on the tcp_pcb connection
 * @param  p: pointer on the received pbuf
 * @param  err: error information regarding the reveived pbuf
 * @retval err_t: error code
 */
err_t tcp_echoclient_received(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    struct tcp_echoclient_struct *ec;
    err_t ret_err;

    LWIP_ASSERT("arg != NULL", arg != NULL);

    ec = (struct tcp_echoclient_struct *)arg;

    /* if we receive an empty tcp frame from client => close connection */
    if (p == NULL)
    {
        /* remote host closed connection */
        ec->state = EC_CLOSING;
        if (ec->p == NULL)
        {
            /* we're done sending, close connection */
            tcp_echoclient_connection_close(tpcb, ec);
        }
        else
        {
            /* we're not done yet */
            /* acknowledge received packet */
            tcp_sent(tpcb, tcp_echoclient_sent);

            /* send remaining data*/
            tcp_echoclient_send(tpcb, ec);
        }
        ret_err = ERR_OK;
    }
    /* else : a non empty frame was received from client but for some reason err != ERR_OK */
    else if (err != ERR_OK)
    {
        /* free received pbuf*/
        if (p != NULL)
        {
            ec->p = NULL;
            pbuf_free(p);
        }
        ret_err = err;
    }
    else if (ec->state == EC_CONNECTED)
    {
        /* first data chunk in p->payload */
        ec->state = EC_RECEIVED;

        /* store reference to incoming pbuf (chain) */
        ec->p = p;

        /* initialize LwIP tcp_sent callback function */
        tcp_sent(tpcb, tcp_echoclient_sent);

        /* send back the received data (echo) */
        tcp_echoclient_send(tpcb, ec);

        ret_err = ERR_OK;
    }
    else if (ec->state == EC_RECEIVED)
    {
        /* more data received from client and previous data has been already sent*/
        if (ec->p == NULL)
        {
            ec->p = p;

            /* send back received data */
            tcp_echoclient_send(tpcb, ec);
        }
        else
        {
            struct pbuf *ptr;

            /* chain pbufs to the end of what we recv'ed previously  */
            ptr = ec->p;
            pbuf_chain(ptr, p);
        }
        ret_err = ERR_OK;
    }
    else if (ec->state == EC_CLOSING)
    {
        /* odd case, remote side closing twice, trash data */
        tcp_recved(tpcb, p->tot_len);
        ec->p = NULL;
        pbuf_free(p);
        ret_err = ERR_OK;
    }
    else
    {
        /* unkown ec->state, trash data  */
        tcp_recved(tpcb, p->tot_len);
        ec->p = NULL;
        pbuf_free(p);
        ret_err = ERR_OK;
    }
    return ret_err;
}

/**
 * @brief  This function implements the tcp_sent LwIP callback (called when ACK
 *         is received from remote host for sent data)
 * @param  arg: pointer on a argument for the tcp_pcb connection
 * @param  tpcb: pointer on the tcp_pcb connection
 * @param  len: Length of the transferred data
 * @retval None
 */
err_t tcp_echoclient_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
    struct tcp_echoclient_struct *ec;

    LWIP_UNUSED_ARG(arg);
    LWIP_UNUSED_ARG(tpcb);
    LWIP_UNUSED_ARG(len);

    ec = (struct tcp_echoclient_struct *)arg;
    ec->retries = 0;

    if (ec->p != NULL)
    {
        /* still got pbufs to send */
        tcp_sent(tpcb, tcp_echoclient_sent);
        tcp_echoclient_send(tpcb, ec);
    }
    else
    {
        /* if no more data to send and client closed connection*/
        if (ec->state == EC_CLOSING)
            tcp_echoclient_connection_close(tpcb, ec);
    }
    return ERR_OK;
}

/**
 * @brief  This function is used to send data for tcp connection
 * @param  tpcb: pointer on the tcp_pcb connection
 * @param  es: pointer on echo_state structure
 * @retval None
 */
void tcp_echoclient_send(struct tcp_pcb *tpcb, struct tcp_echoclient_struct *ec)
{
    struct pbuf *ptr;
    err_t wr_err = ERR_OK;

    while ((wr_err == ERR_OK) &&
           (ec->p != NULL) &&
           (ec->p->len <= tcp_sndbuf(tpcb)))
    {

        /* get pointer on pbuf from es structure */
        ptr = ec->p;

        /* enqueue data for transmission */
        wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);

        if (wr_err == ERR_OK)
        {
            u16_t plen;
            u8_t freed;

            plen = ptr->len;

            /* continue with next pbuf in chain (if any) */
            ec->p = ptr->next;

            if (ec->p != NULL)
            {
                /* increment reference count for es->p */
                pbuf_ref(ec->p);
            }

            /* chop first pbuf from chain */
            do
            {
                /* try hard to free pbuf */
                freed = pbuf_free(ptr);
            } while (freed == 0);
            /* we can read more data now */
            tcp_recved(tpcb, plen);
        }
        else if (wr_err == ERR_MEM)
        {
            /* we are low on memory, try later / harder, defer to poll */
            ec->p = ptr;
        }
        else
        {
            /* other problem ?? */
        }
    }
}

/**
 * @brief  This function implements the tcp_err callback function (called
 *         when a fatal tcp_connection error occurs.
 * @param  arg: pointer on argument parameter
 * @param  err: not used
 * @retval None
 */
void tcp_echoclient_error(void *arg, err_t err)
{
    struct tcp_echoclient_struct *ec;

    LWIP_UNUSED_ARG(arg);
    LWIP_UNUSED_ARG(err);

    ec = (struct tcp_echoclient_struct *)arg;
    if (ec != NULL)
    {
        /*  free ec structure */
        mem_free(ec);
    }
}

/**
 * @brief  This function implements the tcp_poll LwIP callback function
 * @param  arg: pointer on argument passed to callback
 * @param  tpcb: pointer on the tcp_pcb for the current tcp connection
 * @retval err_t: error code
 */
err_t tcp_echoclient_poll(void *arg, struct tcp_pcb *tpcb)
{
    err_t ret_err;
    struct tcp_echoclient_struct *ec;

    ec = (struct tcp_echoclient_struct *)arg;
    if (ec != NULL)
    {
        if (ec->p != NULL)
        {
            tcp_sent(tpcb, tcp_echoclient_sent);
            /* there is a remaining pbuf (chain) , try to send data */
            tcp_echoclient_send(tpcb, ec);
        }
        else
        {
            /* no remaining pbuf (chain)  */
            if (ec->state == EC_CLOSING)
            {
                /*  close tcp connection */
                tcp_echoclient_connection_close(tpcb, ec);
            }
        }
        ret_err = ERR_OK;
    }
    else
    {
        /* nothing to be done */
        tcp_abort(tpcb);
        ret_err = ERR_ABRT;
    }
    return ret_err;
}

/**
 * @brief  This functions closes the tcp connection
 * @param  tcp_pcb: pointer on the tcp connection
 * @param  ec: pointer on echo_state structure
 * @retval None
 */
void tcp_echoclient_connection_close(struct tcp_pcb *tpcb, struct tcp_echoclient_struct *ec)
{
    /* remove all callbacks */
    tcp_arg(tpcb, NULL);
    tcp_sent(tpcb, NULL);
    tcp_recv(tpcb, NULL);
    tcp_err(tpcb, NULL);
    tcp_poll(tpcb, NULL, 0);

    /* delete es structure */
    if (ec != NULL)
    {
        mem_free(ec);
    }

    /* close tcp connection */
    tcp_close(tpcb);
}
