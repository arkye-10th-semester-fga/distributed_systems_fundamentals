// CounterCliente.java
import java.rmi.*;
public class CounterClient {
public static void main(String args[]) {
try {
	String host = args[0];
	Counter c = (Counter)
		Naming.lookup("rmi://"+host+"/Contador");
	System.out.println("Contador: "+c.nextValue());
} catch (Exception e) {
	System.out.println("Erro: "+e.getMessage());}
}
}
