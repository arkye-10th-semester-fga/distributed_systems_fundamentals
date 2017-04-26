// example_Counter.java
import java.rmi.*;
import java.rmi.server.*;
public class example_Counter
extends UnicastRemoteObject
implements Counter {
private int value;
public int nextValue() throws RemoteException {
	return value++;}
public example_Counter() throws RemoteException {
	super();
	value = 0;}
}
