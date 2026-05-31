// SO FAR UNUSED !!

#include <stdio.h>

#include "miobot.h"

extern struct _rep_list_item;
extern struct _rep_server;

struct _rep_list_item {
    u64snowflake user;
    int16_t rep;
    time_t expiry;
    struct _rep_list_item *next;
};

struct _rep_server {
    u64snowflake server;
    struct _rep_list_item *list;
    struct _rep_server *next;
};

static struct _rep_server *_servers;

void _update_rep_cache () {

    // todo: go over all users loaded, save their data, clear server if no users
    for (struct _rep_server *serv = _servers; serv; serv = serv->next) {

        for (struct _rep_list_item *item = serv->list; item;
             item                        = item->next) {}
    }
}

int get_reputation (u64snowflake server, u64snowflake user) {}
