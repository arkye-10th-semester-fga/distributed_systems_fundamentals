21/03/2017 - Terça-feira

# Diálogo UDP

* Servidor orientado à conexão X não orientado à conexão
* Servidor concorrente X iterativo
* Servidor stateful X stateless
	* Eficiência X Confiabilidade
* Servidor apenas como Servidor OU também como Cliente
* Diálogos entre cliente e servidor em geral, envolvem:
	* Protocolo ou regra de comunicação (UDP)
	* Endereços do cliente e do servidor (sockaddr_in)

## UDP (RFC 768)

* Simples extensão do protocolo IP
* Multiplexação/demultiplexação do acesso à camada de rede
* Não-orientado à conexão
* Muitas aplicações C/S baseadas em req/resp usam UDP
	* Evitar o overhead associado ao estabelecimento e encerramento da conexão

## Cabeçalho UDP

* Cabeçalho de 8 bytes, seguido dos dados
	* 64 bits
		* Porta origem
		* Porta destino
		* Tamanho UDP
		* Checksum UDP
* Usa sockets UDP nas entidades comunicantes
* Funções envolvidas (mais comuns):
	* `socket()`
	* `bind()`
	* `recvfrom()` - Equivalente ao `scanf()` para rede
	* `sendto()`

## Ordenação de Bytes

* Possibilidades de representação:
	* big endian
		* `1,2|3,4` --> `0x1234`
	* little endian
		* `3,4|1,2` --> `0x1234`
* Macros para ordenação de bytes:
	* Host p/ Rede (curto) --> `u_short htons (u_short hostshort)`
	* Host p/ Rede (longo) --> `u_long htonl (u_long hostlong)`
	* Rede p/ Host (curto) --> `u_short ntohs (u_short netshort)`
	* Rede p/ Host (longo) --> `u_long ntohl (u_long netlong)`

# Diálogo TCP

* Interação Orientada à conexão
	* Usa sockets TCP nas entidades comunicantes
	* Funções envolvidas (mais comuns):
		* `socket()`
		* `connect()`
		* `send()`
		* `recv()`
		* `bind()`
		* `listen()`
		* `accept()`
		* `close()`

## Cabeçalho TCP

* Source port
* Destination port
* Sequence number
* Acknowledgement number

## Conexão TCP

* Ciclo de vida:
	* Estabelecimento da conexão
		* Three-Way Handshake
	* Transferência de dados
	* Encerramento da conexão

## API Socket

```C
result = connect(socket, endRemoto, tamEndRemoto);
result = send(socket, msg, msglen, flags);
// ...
novoSocket = accept(socket, endRemoto, tamEndRemoto);
```

## Código: `tcpServer2.c`

* Iterativo
* `SOCK_STREAM`: TCP
* `inet_addr(argv[1])`: Converte String para número interno
* `htons(atoi(argv[2]))`: Converte String para número interno
* `for(;;)`: Age como daemon
* `endCli`: Dados do cliente, recebido pelo daemon
* `atende_cliente()`: Recebe dados do cliente

## Código `tcpServer3.c`

* Concorrente
* `pid = fork()`: cria processo filho
* `pid == 0`: Processo filho chama `atende_cliente()`
* `pid > 0`: Processo pai permanece escutando

* Apache:
	* Inicia com +- 10 processos filhos, cada filho suporta +- 50 clientes
* Servidor de e-mail:
	* Escuta na porta 25
	* Servidor cria processo filho para atender cliente
* Spam:
	* Bombardeia máquina com e-mails, sobrecarregando os processos filhos
	* Firewall no gateway impede spams
	* Lista-negra no servidor também

# Diálogo Full-Duplex: Uso do Select

## Servidor de Chat

* Clientes se conectam ao servidor
	* Cada cliente emite/recebe em socket
	* Dados transmitem de um socket para outro
* Grupo
	* Conjuntos de sockets que compõem a thread
	* Cada entrada espalha para os demais (broadcast)
