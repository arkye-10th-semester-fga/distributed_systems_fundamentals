**Nome**: Jonathan Henrique Maia de Moraes

**Matrícula**: 12/0122553

**Disciplina**: Fundamentos de Sistemas Distribuídos (206610)

**Professor**: Fernando W. Cruz

**Data**: 04/04/2017

# Laboratório 02 - Construindo Aplicações Distribuídas Usando RPC

## Introdução

Este relatório visa documentar a prática de construção de 2 aplicações que, em conjunto, permitem a execução distribuída de cálculos aritméticos simples. Tais aplicações foram geradas com o auxílio da ferramenta RPC que estabelece todas as configurações de rede automaticamente, de acordo com um arquivo de configuração base.

O usuário se comunica com a aplicação cliente, que este por sua vez envia os dados fornecidos pelo usuário para o servidor que calcula o valor e devolve ao cliente, onde, por fim, apresenta o resultado ao usuário.

## Descrição da Solução

### Compilação das Aplicações

Foi utilizado o compilador **gcc** versão **5.4.0** e o **GNU Make** versão **4.1** em um ambiente que possui como sistema operacional **Ubuntu** versão **16.04**. O comando para compilação utilizada foi `make`.

### Arquivo de Configuração (`calc.x`)

1. **Síntaxe de Execução**:
```
 rpcgen -a <arquivo_de_configuração>
```
	* **arquivo_de_configuração**: Caminho para o arquivo de configuração RPC
	* **Exemplo de Síntaxe**: `rpcgen -a calc.x`
1. **Criação da Arquitetura-Base RPC**: É gerado os arquivos-base para um projeto de comunicação cliente/servidor com a ferramenta RPC. **CUIDADO**: Tal comando também sobrescreve os arquivos-base previamente configurados! Lembre-se de fazer backup dos códigos e algoritmos desenvolvidos.

### Servidor (`calc_server.c`, `calc_svc.c`, `calc_xdr.c`, `calc.h`)

1. **Síntaxe de Execução**:
```
 ./<executável>
```
	* **executável**: nome da aplicação
	* **Exemplo de Síntaxe**: `./calc_server`
1. **Configuração RPC de Servidor**: É realizado as configurações de servidor na rede local da máquina. Tal procedimento foi gerado automaticamente pelo RPC e sua descrição está contida em `calc_svc.c`.
1. **Cálculo Aritmético**: De acordo com a chamada do cliente, o servidor realiza a operação aritmética correspondente e retorna o resultado para o cliente que requisitou a operação. A descrição deste procedimento está contida em `calc_server.c`.


### Cliente (`calc_client.c`, `calc_clnt.c`, `calc_xdr.c`, `calc.h`)

1. **Síntaxe de Execução**:
```
 ./<executável> <ip_servidor> <valor1> <operador> <valor2>
```
	* **executável**: nome da aplicação
	* **ip_servidor**: endereço do servidor
	* **valor1**: valor numérico de ponto flutuante
	* **operador**: operador aritmético: `+`, `-`, `x`, `"*"` ou `/`
	* **Exemplo de Síntaxe**: `./calc_client localhost 30 x 20`
1. **Validação dos Argumentos**: Caso o número de argumentos seja menor que **5**, a aplicação informa a síntaxe esperada e encerra. Argumentos excedentes são desconsiderados.
1. **Confiração RPC de Cliente com protocolo UDP**: É realizado as configurações entre cliente e servidor com o protocolo UDP. Tal procedimento foi gerado automaticamente pelo RPC e sua descrição está contida em `calc_clnt.c`. A chamada para o procedimento foi realizada manualmente e está contida em `calc_client.c`.
1. **Encaminhamento de Valores**: De acordo com o operador definido pelo usuário, é encaminhado ao servidor os dois valores recebidos para a função do operador descrito.
1. **Apresentação do Resultado**: É informado ao usuário o resultado da operação realizada pelo servidor.

## Códigos

Pelo fato dos arquivos `calc_clnt.c`, `calc_svc.c`, `calc_xdr.c` e `calc.h` terem sido gerados automaticamente pelo RPC, será omitido seu código nesse relatório. O arquivo `Makefile.calc` foi renomeado para `Makefile`, sua operação `make clean` foi renomeado para `make clean-all` e uma nova operação `make clean` foi criada.

### `calc.x`

```
struct operands
{
	double first_value;
	double second_value;
};

program CALC
{
	version VERSION
	{
		double ADD(operands) = 1;
		double SUB(operands) = 2;
		double MULTIPLY(operands) = 3;
		double DIVIDE(operands) = 4;
	} = 100;
} = 55555555;
```

-----

### `calc_server.c`

```C
// Jonathan Henrique Maia de Moraes - 12/0122553
// Github: @arkye

#include "calc.h"

double *
add_100_svc(operands *argp, struct svc_req *rqstp)
{
	static double result;

	fprintf(stdout, "REQUEST: %.2lf + %.2lf\n",
		argp->first_value, argp->second_value);

	result = argp->first_value + argp->second_value;

	return &result;
}

double *
sub_100_svc(operands *argp, struct svc_req *rqstp)
{
	static double result;

	fprintf(stdout, "REQUEST: %.2lf - %.2lf\n",
		argp->first_value, argp->second_value);

	result = argp->first_value - argp->second_value;

	return &result;
}

double *
multiply_100_svc(operands *argp, struct svc_req *rqstp)
{
	static double result;

	fprintf(stdout, "REQUEST: %.2lf * %.2lf\n",
		argp->first_value, argp->second_value);

	result = argp->first_value * argp->second_value;

	return &result;
}

double *
divide_100_svc(operands *argp, struct svc_req *rqstp)
{
	static double result;

	fprintf(stdout, "REQUEST: %.2lf / %.2lf\n",
		argp->first_value, argp->second_value);

	result = argp->first_value / argp->second_value;

	return &result;
}
```

-----

### `calc_client.c`

```C
// Jonathan Henrique Maia de Moraes - 12/0122553
// Github: @arkye

#include <stdio.h>
#include "calc.h"

double calculate(CLIENT *client, double first_value,
	char operator, double second_value);

int main (int argc, char *argv[])
{
	if(argc < 5)
	{
		printf("usage: %s server_host num1 operator num2\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	CLIENT *client;
	client = clnt_create(argv[1], CALC, VERSION, "udp");
	if(client == (CLIENT *) NULL)
	{
		clnt_pcreateerror(argv[1]);
		exit(EXIT_FAILURE);
	}

	double first_value = strtod(argv[2], NULL);
	char operator = argv[3][0];
	double second_value = strtod(argv[4], NULL);
	double result = calculate(client, first_value, operator, second_value);

	printf("%.2lf %c %.2lf = %.2lf\n", first_value, operator,
		second_value, result);

	return 0;
}

double calculate(CLIENT *client, double first_value,
	char operator, double second_value)
{
	operands values;
	values.first_value = first_value;
	values.second_value = second_value;

	double* result;
	if(operator == '+')
	{
		result = add_100(&values, client);
	}
	else if(operator == '-')
	{
		result = sub_100(&values, client);
	}
	else if(operator == 'x' || operator == '*')
	{
		result = multiply_100(&values, client);
	}
	else if(operator == '/')
	{
		result = divide_100(&values, client);
	}
	else
	{
		fprintf(stderr, "operator \"%c\" is invalid!\n", operator);
		fprintf(stderr,
			"if your operator is \"*\", consider to use \"x\" instead\n");
		exit(EXIT_FAILURE);
	}

	if(result == NULL)
	{
		fprintf(stderr, "RPC Calling Problem\n");
		exit(EXIT_FAILURE);
	}

	return *result;
}
```

-----

### `Makefile`

```
# Linhas do Makefile gerados automaticamente pelo RPC
(...)
clean:
	$(RM) core $(OBJECTS_CLNT) $(OBJECTS_SVC) $(CLIENT) $(SERVER)

clean-all:
	$(RM) core $(TARGETS) $(OBJECTS_CLNT) $(OBJECTS_SVC) $(CLIENT) $(SERVER)
```

-----

## Considerações Finais

Foi possível perceber que desenvolver um sistema cliente/servidor utilizando da ferramenta RPC é uma tarefa de alta simplicidade, se comparada com o desenvolvimento manual realizado no laboratório anterior.

Porém, devido à ferramenta estabelecer uma arquitetura pré-definida, tal desenvolvimento está limitado à interface fornecida pelo RPC. Caso seja necessário aplicar configurações mais complexas de rede, é bem provável que será necessário modificar os arquivos-base ou até mesmo considerar não utilizar do RPC para tal desenvolvimento.
