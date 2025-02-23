#ifndef H_CLIENT_UTILS
#define H_CLIENT_UTILS

#include "bencode.h"
#include "client.h"

int protocol(struct tracker_properties*, char*);
int hostname(struct tracker_properties*, char*);
int port(struct tracker_properties*, char*);
int path(struct tracker_properties*, char*);

#endif
