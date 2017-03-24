// Jonathan Henrique Maia de Moraes - 12/0122553
// Github: @arkye
// Based in 'tcpClient' code made by Fernando W Cruz

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <math.h> // -lm
#define MESSAGE_LENGTH 100

int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		printf("Input expected: <server_ip> <server_socket>\n");
		exit(1);
	}

	// Open TCP Socket
	int socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
	// AF_INET = ipv4, SOCK_STREAM = TCP, 0 = Default Protocol
	if(socket_descriptor < 0)
	{
		printf("%s: Can\'t Open Socket\n", argv[0]);
		exit(1);
	}

	// Fill Server Info
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET; // ipv4
	server_address.sin_addr.s_addr = inet_addr(argv[1]); // ip
	server_address.sin_port = htons(atoi(argv[2])); // socket

	// Connect into the Server Socket
	if(connect(socket_descriptor, (struct sockaddr*) &server_address, sizeof(server_address)) < 0)
	{
		printf("%s: Can\'t Connect Into Server %s \n", argv[0], argv[1]);
		exit(1);
	}

	// Communicate With Server
	char message[MESSAGE_LENGTH];
	memset((char*) &message, 0x0, MESSAGE_LENGTH);
	double* result = malloc(sizeof(double));
	while(1)
	{
		printf("[Client] => ");
		if(fgets(message, MESSAGE_LENGTH, stdin) == NULL) break; // Input of the Equation
		send(socket_descriptor, message, strlen(message), 0); // Send Equation

		if(strncmp(message, "END", 3) == 0) break;

		if(recv(socket_descriptor, result, sizeof(double), 0) >= 0)
		{
			printf("[%s:%u] => %.2lf\n", inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port), *result);
		}
	}
	fprintf(stdout, "Finishing Connection With %s:%u ...\n", inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port));
	close(socket_descriptor);
	return 0;
}
