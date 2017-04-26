// Counter.java
import java.rmi.*;
public interface Counter
extends Remote {
	int nextValue() throws RemoteException;
}
