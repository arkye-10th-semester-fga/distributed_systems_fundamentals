// CounterServer.java
import java.rmi.*;
public class CounterServer {
public static void main(String args[])  {
try { Counter c = new example_Counter();
	Naming.rebind("Contador", c);
	System.out.println("Counter ready.");}
catch (Exception e) {
	System.out.println("Erro:"+e.getMessage());}
}
}
