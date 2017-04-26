import java.rmi.registry.Registry;
import java.rmi.registry.LocateRegistry;
import java.rmi.RemoteException;

public class CalculatorClient {

	public static void main(String args[]) {
		boolean withErrors = false;
		String host = null;
		Registry registry = null;
		Calculator stub = null;

		try {
			host = args[0];
			registry = LocateRegistry.getRegistry(host);
			stub = (Calculator) registry.lookup("calculator");
		} catch (Exception e) {
			withErrors = true;
			System.err.println("Unknown hostname!");
		}

		double firstValue = 0.0;
		try {
			firstValue = Double.parseDouble(args[1]);
		} catch (Exception e) {
			withErrors = true;
			System.err.println("The first operand is not a number!");
		}

		char operator = args[2].charAt(0);
		char[] validOperators = { '+', '-', '/', '*'};
		boolean operatorIsValid = false;
		for(char validOperator : validOperators) {
			if(operator == validOperator) {
				operatorIsValid = true;
				break;
			}
		}

		if(!operatorIsValid) {
			System.err.println("Unknown operator!" +
				"\nIf you want to multiply, insert the * with quotes:" +
				"\nEx: 5 \'+\' 3");
			withErrors = true;
		}

		double secondValue = 0.0;
		try {
			secondValue = Double.parseDouble(args[3]);
		} catch (Exception e) {
			withErrors = true;
			System.err.println("The second operand is not a number!");
		}

		if(!withErrors) {
			try {
				double answer = stub.calculate(firstValue, operator, secondValue);
				System.out.println(firstValue + " " + operator + " "
					+ secondValue + " = " + answer);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}
}
