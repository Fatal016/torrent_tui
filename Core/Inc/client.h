#ifndef H_CLIENT
#define H_CLIENT

#include <stdint.h>

#include "bencode.h"

#define MALFORMED_PROTOCOL 100
#define INVALID_PROTOCOL 2

#define htonll(x) (((uint64_t)htonl((x) & 0xFFFFFFFF) << 32) | htonl((x) >> 32))
#define ntohll(x) (((uint64_t)ntohl((x) & 0xFFFFFFFF) << 32) | ntohl((x) >> 32))

struct tracker_properties {
    char* protocol;
    char* hostname;
    char* port;
    char* path;
};

int get_tracker(struct bencode_module*, struct tracker_properties*);
int capture_tracker_properties(struct bencode_module*, struct tracker_properties*, int*);

struct connect_request {
	uint64_t protocol_id;
	uint32_t action;
	uint32_t transaction_id;
} __attribute__((packed));

struct connect_response {
	uint32_t action;
	uint32_t transaction_id;
	uint64_t connection_id;
} __attribute__((packed));

struct announce_request {
	uint64_t* connection_id;
	uint32_t action;
	uint32_t* transaction_id;
	char* info_hash;
	char* peer_id;
	uint64_t* downloaded;
	uint64_t* left;
	uint64_t* uploaded;
	uint64_t event;
//	uint64_t ip_address;
//	uint64_t key;
	uint64_t num_want;
	uint64_t port;
} __attribute__((packed));

#endif
