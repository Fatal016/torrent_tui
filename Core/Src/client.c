#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <netdb.h>
#include <sys/socket.h>

#include "../Inc/client.h"
#include "../Inc/client_utils.h"

int capture_tracker_properties(struct bencode_module *bencode, struct tracker_properties *props, int *tracker_index) {

	int result;

	result = protocol(props, bencode->announce_list[*tracker_index]);
	if (result != PARSE_SUCCESS) {
		return result;
	}

	return 0;

//	result = hostname(props, protocol_end, &hostname_start)
}

int get_tracker(struct bencode_module *bencode, struct tracker_properties *props) {
	
	int tracker_index, result;

	if (bencode->announce_list != NULL) {
		for (tracker_index = 0; tracker_index < bencode->announce_list_index; tracker_index++) {
			result = capture_tracker_properties(bencode, props, &tracker_index);
		
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

				char *return_buffer = (char *)malloc(16 * sizeof(char));

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
                    .action = (uint32_t)return_buffer,
                    .transaction_id = (uint32_t)(return_buffer + 4),
                    .connection_id = (uint64_t)(return_buffer + 8)
                };

				printf("Response:\n\tAction: %x\n\tTransaction ID: %x\n\tConnection ID: %lx\n", connect_res.action, htonl(connect_res.transaction_id), htonll(connect_res.connection_id));
			
				if (transaction_id == connect_res.transaction_id) {
                    printf("Transaction IDs match!\n");
                } else {
                    printf("Transaction IDs do not match...\n");
                    continue;
                }

				struct announce_request announce_req = {
					.connection_id = (char)connect_res.connection_id,
					.action = 1,
					.transaction_id = (char)connect_res.transaction_id,
					.info_hash = bencode->info_hash,
					.peer_id = 
				}


			}
		}
	}
	return 0;
}
