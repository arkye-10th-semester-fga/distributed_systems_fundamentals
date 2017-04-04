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
