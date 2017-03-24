// Jonathan Henrique Maia de Moraes - 12/0122553
// Github: @arkye
// Based in 'udpClient' code made by Fernando W Cruz

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

int main(int argc, char *argv[])
{
	if(argc != 4)
	{
		printf("Input expected: <server_ip> <server_socket> \"equation\"\n");
		exit(1);
	}

	// Open UDP Socket
	int socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
	// AF_INET = ipv4, SOCK_DGRAM = UDP, 0 = Default Protocol
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

	// Fill Client Info
	struct sockaddr_in client_address;
	client_address.sin_family = AF_INET; // ipv4
	client_address.sin_addr.s_addr = htonl(INADDR_ANY); // all available ip
	client_address.sin_port = htons(0); // use socket available between 1024-5000

	// Bind into the Client Socket
	if(bind(socket_descriptor, (struct sockaddr*) &client_address, sizeof(client_address)) < 0)
	{
		printf("%s: Can\'t Bind Into Socket %s \n", argv[0], argv[2]);
		exit(1);
	}

	// Send Equation
	if(sendto(socket_descriptor, argv[3], strlen(argv[3]), 0, (struct sockaddr*) &server_address, sizeof(server_address)) < 0)
	{
		printf("%s: Can't Send The Following Data: %s \n", argv[0], argv[3]);
		exit(1);
	}
	printf("[Client] => %s\n", argv[3]);

	double* result = malloc(sizeof(double));
	unsigned int server_size = sizeof(server_address);

	if(recvfrom(socket_descriptor, result, sizeof(double), 0, (struct sockaddr*) &server_address, &server_size) >= 0)
	{
		printf("[%s:%u] => %.2lf\n", inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port), *result);
	}
}
