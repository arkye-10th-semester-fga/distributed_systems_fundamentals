import java.rmi.registry.Registry;
import java.rmi.registry.LocateRegistry;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class CalculatorServer implements Calculator {

	public CalculatorServer(){}

	@Override
	public double calculate(double firstValue,
		char operator, double secondValue) {
		double answer;
		switch(operator) {
			case '+':
				answer = firstValue + secondValue;
				break;
			case '-':
				answer = firstValue - secondValue;
				break;
			case '*':
				answer = firstValue * secondValue;
				break;
			case '/':
				answer = firstValue / secondValue;
				break;
			default:
				answer = 0.0;
		}
		return answer;
	}

	public static void main(String args[]) {
		try {
			CalculatorServer server = new CalculatorServer();
			Calculator stub = (Calculator) UnicastRemoteObject.exportObject(server, 0);

			Registry registry = LocateRegistry.getRegistry();
			if(System.getSecurityManager() == null) {
				System.setSecurityManager(new SecurityManager());
			}
			registry.rebind("calculator", stub);

			System.err.println("Server ready.");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
