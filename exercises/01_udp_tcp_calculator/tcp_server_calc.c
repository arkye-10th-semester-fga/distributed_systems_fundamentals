// Jonathan Henrique Maia de Moraes - 12/0122553
// Github: @arkye
// Based in 'tcpServer3' code made by Fernando W Cruz

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
#define QUEUE_LENGTH 5
#define MESSAGE_LENGTH 100

void serve_client(int descriptor, struct sockaddr_in client_address);

int main(int argc, char *argv[])
{
	if(argc<3)
	{
		printf("Input expected: <ip> <socket>\n");
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

	// Bind into the Server Socket
	if(bind(socket_descriptor, (struct sockaddr*) &server_address, sizeof(server_address)) < 0)
	{
		printf("%s: Can\'t Bind Into Socket %s \n", argv[0], argv[2]);
		exit(1);
	}

	// Hear Socket
	if(listen(socket_descriptor, QUEUE_LENGTH) < 0)
	{
		fprintf(stderr,"Failed to Hear Socket\n");
		exit(1);
	}

	printf("%s: Waiting Data in IP: %s, UDP Socket: %s\n", argv[0], argv[1], argv[2]);
	struct sockaddr_in client_address;
	unsigned int client_size = sizeof(client_address);
	int new_socket_descriptor;
	int pid;
	while(1) // Wait Connection With Client Process
	{
		// Accept Connection
		if((new_socket_descriptor = accept(socket_descriptor, (struct sockaddr*) &client_address, &client_size)) < 0)
		{
			fprintf(stderr, "Failed Connection\n");
			exit(1);
		}

		// Create new Process
		pid = fork();
		if(pid < 0) fprintf(stderr, "Attempt to Create Child Process Failed\n");
		else if(pid == 0)
		{
			// Child Process
			close(socket_descriptor);
			fprintf(stdout, "Client %s: %u connected\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
			serve_client(new_socket_descriptor, client_address);
			exit(0);
		}
		close(new_socket_descriptor);
	}
}

void serve_client(int descriptor, struct sockaddr_in client_address)
{
	char message[MESSAGE_LENGTH];
	int  message_length; // in bytes
	while(1)
	{
		// Reset the Message Buffer
		memset(message, 0x0, MESSAGE_LENGTH);

		// Get the Message
		message_length = recv(descriptor, &message, MESSAGE_LENGTH, 0);
		if(message_length < 0)
		{
			fprintf(stdout, "Can't Get Data \n");
			continue;
		}
		if(strncmp(message, "END", 3) == 0) break;

		// Print Message
		fprintf(stdout, "[%s:%u] => %s\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port), message);

		// Parse the Message
		int i;
		long long first_number = 0;
		long long second_number = 0;
		char operand = '+';
		char operands[] = "+-*/^";
		for(i = 0; i < 5; i++)
		{
			if(strchr(message, operands[i]) != NULL)
			{
				char* token = strtok(message, operands);
				first_number = atoll(token);
				token = strtok(NULL, operands);
				second_number = atoll(token);
				operand = operands[i];
			}
		}

		// Calculate Result
		double result = 0.0;
		if(operand == '+')
		{
			result = first_number+second_number;
		}
		else if(operand == '-')
		{
			result = first_number-second_number;
		}
		else if(operand == '*')
		{
			result = first_number*second_number;
		}
		else if(operand == '/' && second_number != 0)
		{
			result = first_number/second_number;
		}
		else if(operand == '^')
		{
			result = pow(first_number,second_number);
		}

		// Send Result Back To Client
		message_length = send(descriptor, &result, sizeof(double), 0);
	}
	fprintf(stdout, "Finishing Connection With %s:%u ...\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
	close(descriptor);
}
