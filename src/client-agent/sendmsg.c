/* Copyright (C) 2015-2019, Wazuh Inc.
 * Copyright (C) 2009 Trend Micro Inc.
 * All rights reserved.
 *
 * This program is a free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation
 */

#include "shared.h"
#include "agentd.h"
#include "os_net/os_net.h"

static pthread_mutex_t send_mutex;

/* Initialize sender structure */
void sender_init() {
    pthread_mutex_init(&send_mutex, NULL);
}

/* Send a message to the server */
int send_msg(const char *msg, ssize_t msg_length)
{
    ssize_t msg_size;
    char crypt_msg[OS_MAXSTR + 1];
    int retval;
    int error;

    msg_size = CreateSecMSG(&keys, msg, msg_length < 0 ? strlen(msg) : (size_t)msg_length, crypt_msg, 0);
    if (msg_size <= 0) {
        merror(SEC_ERROR);
        return (-1);
    }

    /* Send msg_size of crypt_msg */
    if (agt->server[agt->rip_id].protocol == UDP_PROTO) {
        retval = OS_SendUDPbySize(agt->sock, msg_size, crypt_msg);
#ifndef WIN32
        error = errno;
#endif
    } else {
        w_mutex_lock(&send_mutex);
        retval = OS_SendSecureTCP(agt->sock, msg_size, crypt_msg);
#ifndef WIN32
        error = errno;
#endif
        w_mutex_unlock(&send_mutex);
    }

    if (!retval) {
        agent_state.msg_sent++;
    } else {
#ifdef WIN32
        error = WSAGetLastError();
        merror(SEND_ERROR, "server", win_strerror(error));
#else
        if(error == EPIPE) {
            mdebug2(TCP_EPIPE);
        } else {
            merror(SEND_ERROR, "server", strerror(error));
        }
#endif
        sleep(1);
    }

    return retval;
}
