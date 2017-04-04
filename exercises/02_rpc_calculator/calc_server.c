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
