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
