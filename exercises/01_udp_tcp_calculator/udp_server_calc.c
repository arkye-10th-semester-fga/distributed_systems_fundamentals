// Jonathan Henrique Maia de Moraes - 12/0122553
// Github: @arkye
// Based in 'udpServer' code made by Fernando W Cruz

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
#include <math.h> // FLAG REQUIRED: -lm
#define MESSAGE_LENGTH 100

int main(int argc, char *argv[])
{
	// Validate Arguments
	if(argc < 3)
	{
		printf("Input expected: <ip> <port>\n");
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
	server_address.sin_port = htons(atoi(argv[2])); // port

	// Bind into the Server Socket
	if(bind(socket_descriptor, (struct sockaddr*) &server_address, sizeof(server_address)) < 0)
	{
		printf("%s: Can\'t Bind Into Socket %s \n", argv[0], argv[2]);
		exit(1);
	}

	printf("%s: Waiting Data in IP: %s, UDP Port: %s\n", argv[0], argv[1], argv[2]);
	struct sockaddr_in client_address;
	unsigned int client_size = sizeof(client_address);
	char message[MESSAGE_LENGTH];
	while(1) // Wait Connection With Client Process
	{
		// Reset the Buffers
		memset(message, 0x0, MESSAGE_LENGTH);

		// Get the Message
		if(recvfrom(socket_descriptor, message, MESSAGE_LENGTH, 0, (struct sockaddr*) &client_address, &client_size) < 0)
		{
			printf("%s: Can't Get Data \n",argv[0]);
			continue;
		}

		// Print Message
		printf("[%s:%u] => %s\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port), message);

		// Parse the Message
		int i;
		double first_number = 0;
		double second_number = 0;
		char operator = '+';
		char operators[] = "+-*/^";
		for(i = 0; i < 5; i++)
		{
			if(strchr(message, operators[i]) != NULL)
			{
				char* token = strtok(message, operators);
				char* endPointer;
				first_number = strtod(token, &endPointer);
				token = strtok(NULL, operators);
				second_number = strtod(token, &endPointer);
				operator = operators[i];
			}
		}

		// Calculate Result
		double result = 0.0;
		if(operator == '+')
		{
			result = first_number+second_number;
		}
		else if(operator == '-')
		{
			result = first_number-second_number;
		}
		else if(operator == '*')
		{
			result = first_number*second_number;
		}
		else if(operator == '/' && second_number != 0)
		{
			result = first_number/second_number;
		}
		else if(operator == '^')
		{
			result = pow(first_number,second_number);
		}

		// Send Result Back To Client
		if(sendto(socket_descriptor, &result, sizeof(result), 0, (struct sockaddr*) &client_address, client_size) >= 0)
		{
			printf("[Server] => %.2lf\n", result); // Inform result
		}
	}
	return 0;
}
