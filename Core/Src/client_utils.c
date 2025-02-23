#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stddef.h>

#include "../Inc/client.h"
#include "../Inc/client_utils.h"

#define DEFAULT_PORT 6969
#define HOSTNAME_SIZE 128
#define PORT_SIZE 128

int protocol(struct tracker_properties *props, char *url) {
	
	int result, protocol_index;
	ptrdiff_t length = 0;

	char valid_protocols[3][5] = {
		"udp",
		"http",
		"https"
	};
	int len_protocols = 3;

	char *protocol_end;

	protocol_end = strstr(url, "://");
	if (protocol_end == NULL) {
		return MALFORMED_PROTOCOL;
	}

	length = protocol_end - url;
	strncpy(props->protocol, url, length);
	props->protocol[length] = '\0';

	for (protocol_index = 0; protocol_index < len_protocols; protocol_index++) {
		if (strcmp(props->protocol, valid_protocols[protocol_index]) == 0) {
			break;
		}
	}
	if (protocol_index == len_protocols) {
		return INVALID_PROTOCOL;
	}

	result = hostname(props, protocol_end);

	return PARSE_SUCCESS;
}

int hostname(struct tracker_properties *props, char *protocol_end) {

	int result;
	
	char *hostname_start, *hostname_end;
	ptrdiff_t length = 0;

	hostname_start = protocol_end + 3 * sizeof(char);
	hostname_end = strstr(hostname_start, ":");

	if (hostname_end == NULL) {
		hostname_end = strstr(hostname_start, "/");
		
		if (hostname_end == NULL) {
			hostname_end = hostname_start + strlen(hostname_start) - sizeof(char);
		}
	}

	length = hostname_end - hostname_start;
	strncpy(props->hostname, hostname_start, length);
	props->hostname[length] = '\0';

	result = port(props, hostname_end);

	return PARSE_SUCCESS;
}

int port(struct tracker_properties *props, char *hostname_end) {

	int result;
	
	char *port_start, *port_end;
	ptrdiff_t length = 0;

	port_start = hostname_end + sizeof(char);
	port_end = strstr(port_start, "/");

	if (port_end == NULL) {
		port_end = strstr(port_start, "\0");
	} else {
		result = path(props, port_end);
	}

	length = port_end - port_start;

	strncpy(props->port, port_start, length);
	props->port[length] = '\0';

	if (strcmp(props->port, "") == 0) {
		if (strcmp(props->protocol, "http") == 0) {
			strcpy(props->port, "80");
		} else if (strcmp(props->protocol, "https") == 0) {
			strcpy(props->port, "443");
		} else {
			return -1;
		}
	}

	printf("Port: %s\n", props->port);

	return PARSE_SUCCESS;
}

int path(struct tracker_properties *props, char *port_end) {
	
	int result;

	strcpy(props->path, port_end);

	return PARSE_SUCCESS;
}
