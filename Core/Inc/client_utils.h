#ifndef H_CLIENT_UTILS
#define H_CLIENT_UTILS

#include "bencode.h"
#include "client.h"


/* default values */
#define DEFAULT_PORT 6969

/* parameters limits */
#define HOSTNAME_SIZE_LIMIT 255
//#define 


/* error codes */
//#define MALFORMED_PROTOCOL 	10
#define MALFORMED_HOSTNAME 	11
#define MALFORMED_PORT		12
#define MALFORMED_PATH		13


int protocol(struct tracker_properties*, char*);
int hostname(struct tracker_properties*, char*);
int port(struct tracker_properties*, char*);
int path(struct tracker_properties*, char*);

#endif
