import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Calculator extends Remote {
	double calculate(double firstValue,
		char operator, double secondValue) throws RemoteException;
}
