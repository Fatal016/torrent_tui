#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <netdb.h>
#include <sys/socket.h>
#include <curl/curl.h>

#include "../Inc/client.h"
#include "../Inc/client_utils.h"

int capture_tracker_properties(struct bencode_module *bencode, struct tracker_properties *props, int *tracker_index) {

	int result;

	result = protocol(props, bencode->announce_list[*tracker_index]);
	if (result != PARSE_SUCCESS) {
		return result;
	}

	return PARSE_SUCCESS;
}

int get_tracker(struct bencode_module *bencode, struct tracker_properties *props) {
		
	int tracker_index, result;

	/* defaulting to announce_list if present since entry index 0 is same as announce */
	if (bencode->announce_list != NULL) {
		for (tracker_index = 0; tracker_index < bencode->announce_list_index; tracker_index++) {
			
			/* parse tracker FQDN and capture properties */
			result = capture_tracker_properties(bencode, props, &tracker_index);
		
			/* error handling for parsing FQDN */
			if (result != PARSE_SUCCESS) {
				return result;
			}

			if (strcmp(props->protocol, "udp") == 0) {
				
				int socket_fd;

				uint32_t transaction_id = rand();

				socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
				if (socket_fd < 0) {
					fprintf(stderr, "Error socket: %s\n", strerror(errno));
				}

				uint32_t *return_buffer = (uint32_t *)malloc(128 * sizeof(uint32_t));

				struct connect_request connect_req = {
					.protocol_id = htonll(0x41727101980),
					.action = 0,
					.transaction_id = htonl(transaction_id)
				};

				struct addrinfo hints;
				struct addrinfo *result, *rp;
				int sfd, s;
				ssize_t nread;

				memset(&hints, 0, sizeof(struct addrinfo));
				hints.ai_family = AF_UNSPEC;

				hints.ai_socktype = SOCK_DGRAM;
				hints.ai_flags = 0;
				hints.ai_protocol = 0;

				s = getaddrinfo(props->hostname, props->port, &hints, &result);
				if (s != 0) {
					fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
					continue;
				}

				for (rp = result; rp != NULL; rp = rp->ai_next) {
                    sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
                    if (sfd == -1) continue;

                    if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1) {
                        printf("Success! Transaction ID: %x\n", connect_req.transaction_id);
                        break;                  /* Success */
                    }
                close(sfd);
                }

                if (rp == NULL) {
                    fprintf(stderr, "Could not connect\n");
                    return -2;
                }

                freeaddrinfo(result);


                struct timeval timeout;
timeout.tv_sec = 2;  // 5 seconds timeout
                timeout.tv_usec = 0;

                if (setsockopt(sfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
                    perror("setsockopt");
                    close(sfd);
                    continue;
                }

                if (send(sfd, (void *)&connect_req, sizeof(connect_req), 0) != sizeof(connect_req)) {
                    fprintf(stderr, "partial/failed write\n");
                    return -2;
                }

                nread = recv(sfd, return_buffer, 16, 0);
                if (nread == -1) {
                    perror("read");
                    continue;
                }

                struct connect_response connect_res = {
                    .action = ntohl(*return_buffer),
                    .transaction_id = ntohl(*(return_buffer + 1)),
                    .connection_id = ((uint64_t)ntohl(*(return_buffer + 2)) << 32) | (uint64_t)ntohl(*(return_buffer + 3))
                };

				printf("Response:\n\tAction: %x\n\tTransaction ID: %x\n\tConnection ID: %lx\n", connect_res.action, htonl(connect_res.transaction_id), htonll(connect_res.connection_id));
			
				if (transaction_id == connect_res.transaction_id) {
                    printf("Transaction IDs match!\n");
                } else {
                    printf("Transaction IDs do not match...\n");
                    continue;
                }

				char *peer_id = (char *)malloc(20 * sizeof(char));
				peer_id	= curl_easy_escape(NULL, "TESTPEERIDTESTPEERID", 20);		

				struct announce_request announce_req = {
					.connection_id = &connect_res.connection_id,
					.action = 1,
					.transaction_id = &connect_res.transaction_id,
					.info_hash = bencode->info_hash,
					.peer_id = peer_id,
					.downloaded = 0,
					.left = bencode->info->length,
					.uploaded = 0,
					.event = 0,
					.num_want = -1,
					.port = 6881
				};

				if (send(sfd, (void *)&announce_req, sizeof(announce_req), 0) != sizeof(announce_req)) {
                    fprintf(stderr, "partial/failed write\n");
                    return -2;
                }

                nread = recv(sfd, return_buffer, 32, 0);
                if (nread == -1) {
                    perror("read");
                    continue;
                }
			}
		}
	}
	return 0;
}
