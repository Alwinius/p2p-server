/*
 ============================================================================
 Name        : p2p-server.c
 Author      : Alwin Ebermann
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <string.h>
#include <sys/types.h> // for things like AF_INET, SOCK_DGRAM
#include <sys/socket.h> // for socket()
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <netdb.h>
#include <sys/time.h>

const int BROADCAST_PORT = 1234;

int verify_peer(int peer) {
	// TODO establish TCP connection
	// TODO mark this peer as active else: remove, return 0
	// TODO retrieve list of its peers
	// TODO filter list with existing active peers - ignore duplicates
	// TODO run verify_peer for all other peers (loop)
	// this function will not refresh existing peers
	// TODO return
	return 1;
}

int get_all_peers(int fileorfilepointer) {
	// read all peers from a file and return them as list of peers
	return 1;
}

int add_peer_to_file(int peer) {
	// TODO call get_all_peers
	// TODO check if peer is already there - then update refresh date
	// TODO otherwise add to the end
	return 1;
}

int refresh_peers_from_file(int fileorfilepointer) {
	// TODO go through the list of peers
	// TODO call verify_peer() for all peers
	// TODO remove if not successful
	//TODO otherwise update refresh date
	return 1;
}

int line_to_peer(int peer) {
	// TODO read one line and convert to peer struct
	return 1;
}

int process_broadcast() {
	// TODO read message
	// TODO convert to peer struct
	// TODO check if peer is already in peers_list
	// TODO call verify_peer if not
	return 1;
}

int send_broadcast(void) {

	int bcast_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (bcast_sock < 0) {
		perror("error creating broadcast socket");
		return 1;
	}

	int broadcastEnable=1;
	if(setsockopt(bcast_sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) == -1) {
		perror("error setting socket options");
		return 1;
	}

	struct sockaddr_in s;
    memset(&s, '\0', sizeof(struct sockaddr_in));
    s.sin_family = AF_INET;
    s.sin_port = (in_port_t)htons(BROADCAST_PORT); // TODO decide which port to use for this broadcast
    s.sin_addr.s_addr = htonl(INADDR_BROADCAST);

    char mess[] = "hello hello \n this is a broadcast";
    if(sendto(bcast_sock, &mess, strlen(mess), 0, (struct sockaddr *)&s, sizeof(struct sockaddr_in)) < 0) {
            perror("error sending on broadcast socket");
    		return 1;
    }

    shutdown(bcast_sock, SHUT_RDWR);
	return 0;
}

int main(void) {

	// TODO check config files or command line args and read values

	// TODO prepare data structures to communicate with other peers
	// - list of peers incl. IP, port, refresh_date
	// - start up service to handle requests
	// TODO open random free port and forward requests to service
	// TODO send broadcast messages on network with own IP and port
	// setup new process (or lookup other method) to regularly broadcast and refresh_peers_from_file()

	int r = send_broadcast();
	printf("%d", r);

	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	return EXIT_SUCCESS;
}
