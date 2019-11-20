/*
 * http.c
 *
 *  Created on: 20 Nov 2019
 *      Author: alwin
 */
#include <stdio.h>
#include <sys/un.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h> // for things like AF_INET, SOCK_DGRAM
#include <sys/socket.h> // for socket()
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <netdb.h>

int process_connection(int connection) {
	if (connection == -1 ) {
		perror("error establishing connection with client");
		return -1;
	}

	// lab 3 exercise 3 starting here
	char server_hostname[NI_MAXHOST];
	gethostname(server_hostname,sizeof(server_hostname));

	struct sockaddr client_addr;
	socklen_t addrlen=sizeof(client_addr);
	if (getpeername(connection, &client_addr, &addrlen)== 0){
		char hbuf[NI_MAXHOST];
		if (getnameinfo(&client_addr, addrlen, hbuf, sizeof(hbuf), NULL, 0, 0) == 0)
			printf("connection to %s from client %s established.\n", server_hostname, hbuf);
	} else {
			perror("error getting client name");
	}
	// lab 3 exercise 3 end

	char buffer[1024] = "";
	int e = read(connection, buffer, sizeof(buffer));
	if (e == -1) {
		perror("error receiving data");
		return -1;
	} else if (e == 0) {
		perror("received zero byte message");
		return -1;
	} else {
		char delimiter[] = " ";
		//printf("%s", buffer);
		char *req_type = strtok(buffer, delimiter);
		printf("Request Type is %s\n",req_type);
		char *req_url = strtok(NULL, delimiter);
		req_url++; // this will remove the / in front of the file name
		printf("Request Path is %s\n", req_url);
		if (strcmp(req_type, "GET") != 0) {
			e = shutdown(connection,SHUT_RDWR);
			printf("this is not a get request, ignoring '%s'\n", req_type);
			return -1;
		}
		FILE *fp;
		char file_buffer[255];
		if (strlen(req_url)==0) {
		   fp = fopen("index.html", "r"); 	// show index.html
		} else {
		   fp = fopen(req_url, "r"); // show file from req_url
		}

	   if (fp == NULL) {
		   perror("error opening file");
		   // now we return HTTP 404
		   char response[] = "HTTP/1.1 404 NOT FOUND\r\n\
				   Date: Mon, 26 Apr 2019 16:20:39 GMT\r\n\
				   Server: Apache/2.2.13 (Linux/SUSE) \r\n\
				   Last-Modified: Mon, 26 Apr 2010 14:05:43 GMT\r\n\
				   Connection: close\r\n\
				   Content-Type: text/html\r\n\r\n\
				   404 NOT FOUND";
				   e = write(connection, response, sizeof(response)-1);
	   } else {
			char response[] = "HTTP/1.1 200 OK\r\n\
			Date: Mon, 26 Apr 2019 16:20:39 GMT\r\n\
			Server: Apache/2.2.13 (Linux/SUSE) \r\n\
			Last-Modified: Mon, 26 Apr 2010 14:05:43 GMT\r\n\
			Connection: close\r\n\
			Content-Type: text/html\r\n\r\n";
			e = write(connection, response, sizeof(response)-1);
			while(fscanf(fp, "%s", file_buffer) != EOF )
			{
				e = write(connection, file_buffer, strlen(file_buffer)*sizeof(char));
			}
			fclose(fp);
			if (e == -1)
				perror("error returning message");
			else
				printf("returned message\n");
	   }
	   //return 1;
		e = shutdown(connection,SHUT_RDWR);
		if (e == -1 ) {
			perror("error closing connection");
		}
	}
	return -1;
}
