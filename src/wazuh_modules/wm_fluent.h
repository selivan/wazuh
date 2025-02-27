/*
 * Wazuh Module for Fluent Forwarder
 * Copyright (C) 2015-2019, Wazuh Inc.
 * March 26, 2019.
 *
 * This program is a free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation.
 */

#ifndef WM_FLUENT_H
#define WM_FLUENT_H

#include <openssl/ssl.h>

#define WM_FLUENT_LOGTAG FLUENT_WM_NAME

typedef struct wm_fluent_t {
    unsigned int enabled:1;
    char * tag;
    char * sock_path;
    char * address;
    unsigned short port;
    char * shared_key;
    char * certificate;
    char * user_name;
    char * user_pass;
    int timeout;
    int client_sock;
    SSL_CTX * ctx;
    SSL * ssl;
    BIO * bio;
} wm_fluent_t;

typedef struct wm_fluent_helo_t {
    size_t nonce_size;
    char * nonce;
    size_t auth_size;
    char * auth;
    unsigned int keepalive:1;
} wm_fluent_helo_t;

typedef struct wm_fluent_pong_t {
    unsigned int auth_result:1;
    char * reason;
    char * server_hostname;
    char * shared_key_hexdigest;
} wm_fluent_pong_t;

extern const wm_context WM_FLUENT_CONTEXT;

// Read configuration and return a module (if enabled) or NULL (if disabled)
int wm_fluent_read(xml_node **nodes, wmodule *module);


#endif // WM_FLUENT_H
