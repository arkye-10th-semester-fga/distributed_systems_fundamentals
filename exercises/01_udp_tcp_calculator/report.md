**Nome**: Jonathan Henrique Maia de Moraes
**Matrícula**: 12/0122553
**Disciplina**: Fundamentos de Sistemas Distribuídos (206610)
**Professor**: Fernando W. Cruz
**Data**: 24/03/2017

# Laboratório 01 - Construindo Aplicações Distribuídas Usando Sockets

## Introdução

Este relatório visa documentar a prática de construção de 4 aplicações que, duas a duas, permitem a execução distribuída de cálculos aritméticos simples. Cada par age em estrutura de cliente/servidor, um par utilizando do protocolo UDP e outro par utilizando do protocolo TCP para comunicação.

O usuário se comunica com a aplicação cliente, que este por sua vez envia os dados fornecidos pelo usuário para o servidor que calcula o valor e devolve ao cliente, onde, por fim, apresenta o resultado ao usuário.

## Descrição da Solução

### Compilação das Aplicações

Foi utilizado o compilador **gcc** versão **5.4.0** em um ambiente que possui como sistema operacional **Ubuntu** versão **16.04**. As seguintes _flags_ foram utilizadas: **-lm**, **-O2** e **-Wall**.

### UDP - Cliente (`udp_client_calc.c`)

1. **Síntaxe de Execução**:
```
 ./<executável> <ip do servidor> <porta do servidor> "equação"
```
	* **executável**: nome da aplicação
	* **ip do servidor**: ip utilizado na aplicação `udp_server_calc.c`
	* **porta do servidor**: porta utilizada na aplicação `udp_server_calc.c`
	* **equação**: equação aritmética simples contida dentro de aspas duplas. Os operadores disponíveis para cálculo são: `+` (soma), `-` (subtração), `*` (multiplicação), `/` (divisão) e `^` (potência)
		* **Exemplo 01**: `"100^4"`
		* **Exemplo 02**: `"20.5 +21.5"`
		* **Exemplo 03**: `"33 * 25"`
	* **Exemplo de Síntaxe**: `./udp_client 192.168.0.15 9090 "30-20"`
1. **Validação dos Argumentos**: Caso o número de argumentos seja diferente de **4**, a aplicação informa síntaxe esperada e encerra.
1. **Abertura de _socket_ UDP**: De família IPv4, é aberto um _socket_ UDP que servirá de elo de comunicação entre a aplicação atual e o servidor.
1. **Estruturação do caminho do cliente**: Estabelece vínculo com um ip e porta livres disponíveis na máquina.
1. **Envio de equação para o servidor**: Envia a equação para o ip e porta fornecidos pelo usuário. É apresentado ao usuário a equação fornecida se o envio não apresentar erros.
1. **Recebimento do resultado da equação**: Recebe um valor numérico do servidor que corresponde ao resultado da equação fornecida pelo usuário.
1. **Apresentação do resultado da equação**: Apresenta o resultado ao usuário com precisão de duas casas decimais e encerra.


### UDP - Servidor (`udp_server_calc.c`)

1. **Síntaxe de Execução**:
```
 ./<executável> <ip> <porta>
```
	* **executável**: nome da aplicação
	* **ip**: ip a ser utilizado como servidor
	* **porta**: porta a ser utilizada como servidor
	* **Exemplo de Síntaxe**: `./udp_server 192.168.0.15 9090`
1. **Validação dos Argumentos**: Caso o número de argumentos seja menor que **3**, a aplicação informa a síntaxe esperada e encerra. Argumentos excedentes são desconsiderados.
1. **Abertura de _socket_ UDP**: De família IPv4, é aberto um _socket_ UDP que servirá de elo de comunicação entre a aplicação atual e o cliente.
1. **Estruturação do caminho do servidor**: Estabelece vínculo com o ip e porta fornecidos pelo usuário.
1. **Espera da conexão com o cliente**: Em um _loop_ infinito, o servidor está disponível para conexões.
1. **Recebimento da mensagem**: Uma vez em comunicação com um cliente, é recebido a mensagem (equação aritmética simples) do mesmo. É apresentado ao usuário do servidor a mensagem fornecida pelo usuário cliente se o recebimento for bem sucedido.
1. **Tradução da mensagem em variáveis locais**: A mensagem é diluída em **3** variáveis locais, onde a primeira (`first_number`) e a terceira (`second_number`) são os valores numéricos, e a segunda (`operator`) o operador da equação. Em caso de recebimento de mensagem fora deste padrão, a aplicação agirá inesperadamente.
1. **Cálculo do resultado da equação**: É realizado o cálculo correspondente à equação.
1. **Envio do resultado da equação para o cliente**: Envia o resultado da equação para o ip e porta do cliente (descoberto durante o recebimento da mensagem) e apresenta o resultado ao usuário do servidor com precisão de duas casas decimais. Vale lembrar que o protocolo UDP não estabelece vínculo de conexão entre as máquinas, sendo necessário uma nova chamada do cliente, que criará uma nova conexão, para a realização de um segundo cálculo.

### TCP - Cliente (`tcp_client_calc.c`)

1. **Síntaxe de Execução**:
```
 ./<executável> <ip do servidor> <porta do servidor>
```
	* **executável**: nome da aplicação;
	* **ip do servidor**: ip utilizado na aplicação `tcp_server_calc.c`
	* **porta do servidor**: porta utilizada na aplicação `tcp_server_calc.c`
	* **Exemplo de Síntaxe**: `./tcp_client 192.168.0.15 9090`
1. **Validação dos Argumentos**: Caso o número de argumentos seja menor que **3**, a aplicação informa a síntaxe esperada e encerra. Argumentos excedentes são desconsiderados.
1. **Abertura de _socket_ TCP**: De família IPv4, é aberto um _socket_ TCP que servirá de elo de comunicação entre a aplicação atual e o servidor.
1. **Conexão com o servidor**: Estabelece conexão com o ip e porta fornecidos pelo usuário.
1. **Comunicação com o servidor**: Cliente está propenso ao envio da equação para o servidor. O usuário, ao informar a equação, o mesmo é enviado para o servidor. O cliente aguarda a resposta com o resultado, onde então é apresentado ao usuário com precisão de duas casas decimais. Caso a mensagem de envio seja `END`, a aplicação sairá do _loop_ de comunicação, informará o término da conexão e encerrará.

### TCP - Servidor (`tcp_server_calc.c`)

1. **Síntaxe de Execução**:
```
 ./<executável> <ip> <porta>
```
	* **executável**: nome da aplicação;
	* **ip**: ip a ser utilizado como servidor
	* **porta**: porta a ser utilizada como servidor
	* **Exemplo de Síntaxe**: `./tcp_server 192.168.0.15 9090`
1. **Validação dos Argumentos**: Caso o número de argumentos seja menor que **3**, a aplicação informa a síntaxe esperada e encerra. Argumentos excedentes são desconsiderados.
1. **Abertura de _socket_ TCP**: De família IPv4, é aberto um _socket_ TCP que servirá de elo de comunicação entre a aplicação atual e o cliente.
1. **Estruturação do caminho do servidor**: Estabelece vínculo com o ip e porta fornecidos pelo usuário.
1. **Espera da conexão com o cliente**: Em um _loop_ infinito, o servidor está disponível para conexões de forma concorrente (até **5** conexões simultâneas).
1. **Conexão com um cliente**: Uma vez aceito a conexão com um cliente, um processo filho é criado para gerenciar tal conexão.
1. **Comunicação com um cliente**: Servidor está propenso ao recebimento de mensagens do cliente. Caso a mensagem seja `END`, o processo filho fecha a conexão e morre.
1. **Tradução da mensagem em variáveis locais**: Caso a mensagem não seja `END`, a mesma é diluída em **3** variáveis locais, onde a primeira (`first_number`) e a terceira (`second_number`) são os valores numéricos, e a segunda (`operator`) o operador da equação. Em caso de recebimento de mensagem fora deste padrão, a aplicação agirá inesperadamente.
1. **Cálculo do resultado da equação**: É realizado o cálculo correspondente à equação.
1. **Envio do resultado da equação para o cliente**: Envia o resultado da equação para o cliente e apresenta o resultado ao usuário do servidor com precisão de duas casas decimais. Vale lembrar que o protocolo TCP estabelece vínculo de conexão entre as máquinas, podendo receber sucessivas requisições por parte do cliente de cálculos aritméticos sem a necessidade de uma nova conexão.

## Códigos

### `udp_client_calc.c`

```C
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
#include <math.h> // FLAG REQUIRED: -lm

int main(int argc, char *argv[])
{
	// Validate arguments
	if(argc != 4)
	{
		printf("Input expected: <server_ip> <server_port> \"equation\"\n");
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

	// Fill Client Info
	struct sockaddr_in client_address;
	client_address.sin_family = AF_INET; // ipv4
	client_address.sin_addr.s_addr = htonl(INADDR_ANY); // all available ip
	client_address.sin_port = htons(0); // use port available between 1024-5000

	// Bind into the Client Socket
	if(bind(socket_descriptor, (struct sockaddr*) &client_address,
		sizeof(client_address)) < 0)
	{
		printf("%s: Can\'t Bind Into Socket %s \n", argv[0], argv[2]);
		exit(1);
	}

	// Fill Server Info
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET; // ipv4
	server_address.sin_addr.s_addr = inet_addr(argv[1]); // ip
	server_address.sin_port = htons(atoi(argv[2])); // port

	// Send Equation
	if(sendto(socket_descriptor, argv[3], strlen(argv[3]), 0,
		(struct sockaddr*) &server_address, sizeof(server_address)) < 0)
	{
		printf("%s: Can't Send The Following Data: %s \n", argv[0], argv[3]);
		exit(1);
	}
	printf("[Client] => %s\n", argv[3]); // Inform equation

	// Get Result From Server
	double* result = malloc(sizeof(double));
	unsigned int server_size = sizeof(server_address);
	if(recvfrom(socket_descriptor, result, sizeof(double), 0,
		(struct sockaddr*) &server_address, &server_size) >= 0)
	{
		// Inform result
		printf("[%s:%u] => %.2lf\n", inet_ntoa(server_address.sin_addr),
			ntohs(server_address.sin_port), *result);
	}
}
```

-----

### `udp_server_calc.c`

```C
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
	if(bind(socket_descriptor, (struct sockaddr*) &server_address,
		sizeof(server_address)) < 0)
	{
		printf("%s: Can\'t Bind Into Socket %s \n", argv[0], argv[2]);
		exit(1);
	}

	printf("%s: Waiting Data in IP: %s, UDP Port: %s\n", argv[0],
		argv[1], argv[2]);
	struct sockaddr_in client_address;
	unsigned int client_size = sizeof(client_address);
	char message[MESSAGE_LENGTH];
	while(1) // Wait Connection With Client Process
	{
		// Reset the Buffers
		memset(message, 0x0, MESSAGE_LENGTH);

		// Get the Message
		if(recvfrom(socket_descriptor, message, MESSAGE_LENGTH, 0,
			(struct sockaddr*) &client_address, &client_size) < 0)
		{
			printf("%s: Can't Get Data \n",argv[0]);
			continue;
		}

		// Print Message
		printf("[%s:%u] => %s\n", inet_ntoa(client_address.sin_addr),
			ntohs(client_address.sin_port), message);

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
		if(sendto(socket_descriptor, &result, sizeof(result), 0,
			(struct sockaddr*) &client_address, client_size) >= 0)
		{
			printf("[Server] => %.2lf\n", result); // Inform result
		}
	}
	return 0;
}
```

-----

### `tcp_client_calc.c`

```C
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
#include <math.h> // FLAG REQUIRED: -lm
#define MESSAGE_LENGTH 100

int main(int argc, char *argv[])
{
	// Validate Arguments
	if(argc < 3)
	{
		printf("Input expected: <server_ip> <server_port>\n");
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
	server_address.sin_port = htons(atoi(argv[2])); // port

	// Connect into the Server Socket
	if(connect(socket_descriptor, (struct sockaddr*) &server_address,
		sizeof(server_address)) < 0)
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

		// Input of the Equation
		if(fgets(message, MESSAGE_LENGTH, stdin) == NULL) break;

		send(socket_descriptor, message, strlen(message), 0); // Send Equation

		if(strncmp(message, "END", 3) == 0) break;

		// Get Result From The Server
		if(recv(socket_descriptor, result, sizeof(double), 0) >= 0)
		{
			printf("[%s:%u] => %.2lf\n", inet_ntoa(server_address.sin_addr),
				ntohs(server_address.sin_port), *result);
		}
	}
	fprintf(stdout, "Finishing Connection With %s:%u ...\n",
		inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port));
	close(socket_descriptor);
	return 0;
}
```

-----

### `tcp_server_calc.c`

```C
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
	// Validate Arguments
	if(argc<3)
	{
		printf("Input expected: <ip> <port>\n");
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
	server_address.sin_port = htons(atoi(argv[2])); // port

	// Bind into the Server Socket
	if(bind(socket_descriptor, (struct sockaddr*) &server_address,
		sizeof(server_address)) < 0)
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

	printf("%s: Waiting Data in IP: %s, TCP Port: %s\n", argv[0],
		argv[1], argv[2]);
	struct sockaddr_in client_address;
	unsigned int client_size = sizeof(client_address);
	int new_socket_descriptor;
	int pid;
	while(1) // Wait Connection With Client Process
	{
		// Accept Connection
		if((new_socket_descriptor = accept(socket_descriptor,
			(struct sockaddr*) &client_address, &client_size)) < 0)
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
			fprintf(stdout, "Client %s: %u connected\n",
				inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
			serve_client(new_socket_descriptor, client_address);
			exit(0);
		}
		close(new_socket_descriptor);
	}
}

void serve_client(int descriptor, struct sockaddr_in client_address)
{
	char message[MESSAGE_LENGTH];
	while(1)
	{
		// Reset the Message Buffer
		memset(message, 0x0, MESSAGE_LENGTH);

		// Get the Message
		if(recv(descriptor, &message, MESSAGE_LENGTH, 0) < 0)
		{
			fprintf(stdout, "Can't Get Data \n");
			continue;
		}
		if(strncmp(message, "END", 3) == 0) break;

		// Print Message
		fprintf(stdout, "[%s:%u] => %s\n", inet_ntoa(client_address.sin_addr),
			ntohs(client_address.sin_port), message);

		// Parse the Message
		int i;
		double first_number = 0;
		double second_number = 0;
		char operand = '+';
		char operands[] = "+-*/^";
		for(i = 0; i < 5; i++)
		{
			if(strchr(message, operands[i]) != NULL)
			{
				char* token = strtok(message, operands);
				char* endPointer;
				first_number = strtod(token, &endPointer);
				token = strtok(NULL, operands);
				second_number = strtod(token, &endPointer);
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
		if(send(descriptor, &result, sizeof(double), 0) >= 0)
		{
			fprintf(stdout, "[Server] => %.2lf\n", result);
		}
	}
	fprintf(stdout, "Finishing Connection With %s:%u ...\n",
		inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
	close(descriptor);
}
```

## Resultados de Testes

### Cenários de Teste

1. **Soma**: Calcular `80.50 + 19.50`, onde o resultado deve ser: `100.00`
1. **Subtração**: Calcular `200-100`, onde o resultado deve ser: `100.00`
1. **Multiplicação**: Calcular `20.00* 5`, onde o resultado deve ser: `100.00`
1. **Divisão**: Calcular `2200 /22.0`, onde o resultado deve ser: `100.00`
1. **Potência**: Calcular `10^2`, onde o resultado deve ser: `100.00`

### Via UDP

1. **Estabelecendo Servidor**:
	* **Servidor**:
	```
	$ ./udp_server 192.168.0.15 9090
	./udp_server: Waiting Data in IP: 192.168.0.15, UDP Port: 9090
	```
1. **Soma**:
	* **Cliente**:
	```
	$ ./udp_client 192.168.0.15 9090 "80.50 + 19.50"
	[Client] => 80.50 + 19.50
	[192.168.0.15:9090] => 100.00
	```
	* **Servidor**:
	```
	[192.168.0.15:43031] => 80.50 + 19.50
	[Server] => 100.00
	```
1. **Subtração**:
	* **Cliente**:
	```
	$ /udp_client 192.168.0.15 9090 "200-100"
	[Client] => 200-100
	[192.168.0.15:9090] => 100.00
	```
	* **Servidor**:
	```
	[192.168.0.15:38853] => 200-100
	[Server] => 100.00
	```
1. **Multiplicação**:
	* **Cliente**:
	```
	$ ./udp_client 192.168.0.15 9090 "20.00* 5"
	[Client] => 20.00* 5
	[192.168.0.15:9090] => 100.00
	```
	* **Servidor**:
	```
	[192.168.0.15:55794] => 20.00* 5
	[Server] => 100.00
	```
1. **Divisão**:
	* **Cliente**:
	```
	$ ./udp_client 192.168.0.15 9090 "2200 /22.0"
	[Client] => 2200 /22.0
	[192.168.0.15:9090] => 100.00
	```
	* **Servidor**:
	```
	[192.168.0.15:57328] => 2200 /22.0
	[Server] => 100.00
	```
1. **Potência**:
	* **Cliente**:
	```
	$ ./udp_client 192.168.0.15 9090 "10^2"
	[Client] => 10^2
	[192.168.0.15:9090] => 100.00
	```
	* **Servidor**:
	```
	[192.168.0.15:37044] => 10^2
	[Server] => 100.00
	```
1. **Encerrando Servidor**:
	* **Servidor**:
	```
	^C
	```

### Via TCP

1. **Abrindo Conexão**:
	* **Servidor**:
	```
	$ ./tcp_server 192.168.0.15 9090
	./tcp_server: Waiting Data in IP: 192.168.0.15, TCP Port: 9090
	```
	* **Cliente**:
	```
	$ ./tcp_client 192.168.0.15 9090
	[Client] =>
	```
	* **Servidor**:
	```
	Client 192.168.0.15: 58766 connected
	```
1. **Soma**:
	* **Cliente**:
	```
	[Client] => 80.50 + 19.50
	[192.168.0.15:9090] => 100.00
	[Client] =>
	```
	* **Servidor**:
	```
	[192.168.0.15:58766] => 80.50 + 19.50
	[Server] => 100.00
	```
1. **Abrindo nova Conexão**:
	* **Cliente2**:
	```
	$ ./tcp_client 192.168.0.15 9090
	[Client] =>
	```
	* **Servidor**:
	```
	Client 192.168.0.15: 58770 connected
	```
1. **Subtração**:
	* **Cliente2**:
	```
	[Client] => 200-100
	[192.168.0.15:9090] => 100.00
	[Client] =>
	```
	* **Servidor**:
	```
	[192.168.0.15:58770] => 200-100
	[Server] => 100.00
	```
1. **Abrindo nova Conexão**:
	* **Cliente3**:
	```
	$ ./tcp_client 192.168.0.15 9090
	[Client] =>
	```
	* **Servidor**:
	```
	Client 192.168.0.15: 58772 connected
	```
1. **Multiplicação**:
	* **Cliente3**:
	```
	[Client] => 20.00* 5
	[192.168.0.15:9090] => 100.00
	[Client] =>
	```
	* **Servidor**:
	```
	[192.168.0.15:58772] => 20.00* 5
	[Server] => 100.00
	```
1. **Abrindo nova Conexão**:
	* **Cliente4**:
	```
	$ ./tcp_client 192.168.0.15 9090
	[Client] =>
	```
	* **Servidor**:
	```
	Client 192.168.0.15: 58774 connected
	```
1. **Divisão**:
	* **Cliente4**:
	```
	[Client] => 2200 /22.0
	[192.168.0.15:9090] => 100.00
	[Client] =>
	```
	* **Servidor**:
	```
	[192.168.0.15:58774] => 2200 /22.0
	[Server] => 100.00
	```
1. **Abrindo nova Conexão**:
	* **Cliente5**:
	```
	$ ./tcp_client 192.168.0.15 9090
	[Client] =>
	```
	* **Servidor**:
	```
	Client 192.168.0.15: 58776 connected
	```
1. **Potência**:
	* **Cliente5**:
	```
	[Client] => 10^2
	[192.168.0.15:9090] => 100.00
	[Client] =>
	```
	* **Servidor**:
	```
	[192.168.0.15:58776] => 10^2
	[Server] => 100.00
	```
1. **Encerrando Conexões**:
	* **Cliente**:
	```
	[Client] => END
	Finishing Connection With 192.168.0.15:9090 ...
	```
	* **Servidor**:
	```
	Finishing Connection With 192.168.0.15:58766 ...
	```
	* **Cliente2**:
	```
	[Client] => END
	Finishing Connection With 192.168.0.15:9090 ...
	```
	* **Servidor**:
	```
	Finishing Connection With 192.168.0.15:58770 ...
	```
	* **Cliente3**:
	```
	[Client] => END
	Finishing Connection With 192.168.0.15:9090 ...
	```
	* **Servidor**:
	```
	Finishing Connection With 192.168.0.15:58772 ...
	```
	* **Cliente4**:
	```
	[Client] => END
	Finishing Connection With 192.168.0.15:9090 ...
	```
	* **Servidor**:
	```
	Finishing Connection With 192.168.0.15:58774 ...
	```
	* **Cliente5**:
	```
	[Client] => END
	Finishing Connection With 192.168.0.15:9090 ...
	```
	* **Servidor**:
	```
	Finishing Connection With 192.168.0.15:58776 ...
	```
	* **Servidor**:
	```
	^C
	```

## Considerações Finais

Estabelecer comunicação entre duas máquinas distintas, ou até mesmo entre dois sistemas ou aplicações distintas, é uma tarefa comum da computação. Realizar tal atividade utilizando da linguagem C permite uma compreensão mais apronfudada sobre os protocolos TCP e UDP e suas implementações.

Para evoluir essa simples calculadora para um sistema distribuído complexo, seja para ganhar performance de cálculo ou para aperfeiçoar a usabilidade e as funcionalidades das aplicações, se faz necessário de um elevado uso de programação concorrente e otimização de fluxo de mensagens.

Entender as funções utilizadas para configuração, conexão e comunicação foi uma tarefa não-trivial mas satisfatório em termos de aprendizado. Ainda assim, há uma dificuldade em aplicar modificações para construir um sistema onde mais do que duas entidades se comunicam entre sí, como um sistema onde dois ou mais clientes interagem e se comunicam através de um ou mais servidores, com confiabilidade e resiliência para as mais diversas situações.
