**Nome**: Jonathan Henrique Maia de Moraes

**Matrícula**: 12/0122553

**Disciplina**: Fundamentos de Sistemas Distribuídos (206610)

**Professor**: Fernando W. Cruz

**Data**: 14/04/2017

# Laboratório 03 - Construindo Aplicações Distribuídas Usando RMI

## Introdução

Este relatório visa documentar a prática de construção de 2 pares de aplicações em linguagens distintas (Java e Ruby) que, separadamente, permitem a execução distribuída de cálculos aritméticos simples. Tais aplicações foram geradas com o auxílio das ferramentas RMI (Java) e DRb (RMI para o Ruby) que estabelecem todas as configurações de rede automaticamente.

O usuário se comunica com a aplicação cliente, que este por sua vez envia os dados fornecidos pelo usuário para o servidor que calcula o valor e devolve ao cliente, onde, por fim, apresenta o resultado ao usuário.

## Descrição da Solução - Java (RMI)

### Compilação das Aplicações

Foi utilizado o compilador **javac** versão **1.8.0_121** e o **GNU Make** versão **4.1** em um ambiente que possui como sistema operacional **Ubuntu** versão **16.04**. O comando para compilação utilizada foi `make`.

### Servidor (`CalculatorServer.java`)

1. **Síntaxe de Execução**:
```
java -Djava.security.manager -Djava.security.policy=server.policy
	CalculatorServer
```
1. **Criação do objeto de interação (_stub_)**: Através da API do RMI, é exportado o objeto `Calculator` com vínculo ao servidor.
1. **Vínculo da chamada de objeto ao servidor**: Através da API do RMI, é feito um vínculo entre o servidor e a chamada de objeto `calculator`.
1. **Informe de sucesso de configuração**: É informado ao usuário do servidor que o servidor está pronto.

### Cliente (`CalculatorClient.java`)

1. **Síntaxe de Execução**:
```
 java CalculatorClient <ip_servidor> <valor1> <operador> <valor2>
```
	* **ip_servidor**: endereço do servidor
	* **valor1, valor2**: valor numérico de ponto flutuante
	* **operador**: operador aritmético: `+`, `-`, `"*"` ou `/`
	* **Exemplo de Síntaxe**: `java CalculatorClient localhost 30 + 20`
1. **Recebe objeto de interação (_stub_) do servidor**: Através da API do RMI, é recebido o registro do servidor e o objeto de interação.
1. **Validação das entradas**: É feito a validação dos valores e do operador de forma a garantir que a chamada para o servidor tenha os objetos corretos.
1. **Pedido de cálculo**: Realiza-se a chamada do método `calculate()` contido no objeto de interação com as entradas do usuário como parâmetro.
1. **Apresentação do resultado**: É informado ao usuário o resultado da operação realizada pelo servidor.

## Códigos

### `server.policy`

```
grant codebase "file:/home/arkye/Workspace/FSD/distributed_systems_fundamentals
	/exercises/03_rmi_calculator" {
	permission java.net.SocketPermission "127.0.0.1", "accept,
	connect, resolve";
	permission java.net.SocketPermission "localhost", "accept,
	connect, resolve";
};
```

-----

### `Calculator.java`

```Java
import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Calculator extends Remote {
	double calculate(double firstValue,
		char operator, double secondValue) throws RemoteException;
}
```

-----

### `CalculatorClient.java`

```Java
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
				double answer = stub.calculate(firstValue, operator,
					secondValue);
				System.out.println(firstValue + " " + operator + " "
					+ secondValue + " = " + answer);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}
}
```

-----

### `CalculatorServer.java`

```Java
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
			Calculator stub = (Calculator)
				UnicastRemoteObject.exportObject(server, 0);

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
```

-----

### `Makefile`

```
# Java Server/Client Makefile

# Parameters
CLIENT = CalculatorClient
SERVER = CalculatorServer
INTERFACE = Calculator
SERVER_POLICY = server.policy

BUILD = javac $(INTERFACE).java $(SERVER).java $(CLIENT).java

EXEC_SERVER = java -Djava.security.manager
	-Djava.security.policy=$(SERVER_POLICY) $(SERVER)

CLEAN = rm *.class

# Targets
all:
	@make build --always-make --no-print-directory

build:
	$(BUILD)

exec-server:
	$(EXEC_SERVER)

clean:
	$(CLEAN)
```

-----

## Descrição da Solução - Ruby (DRb)

### Compilação das Aplicações

Foi utilizado o interpretador **ruby** versão **2.3.1p112** em um ambiente que possui como sistema operacional **Ubuntu** versão **16.04**.

### Servidor (`calc_server.rb`)

1. **Síntaxe de Execução**:
```
ruby calc_server.rb <ip>:<porta>
```
	* **ip**: IP para registro do servidor
	* **porta**: Porta para registro do servidor
	* **Exemplo de Síntaxe**: `ruby calc_server.rb localhost:9000`
1. **Definição do URI**: É definida o URI `druby://` com o ip e porta definido pelo usuário do servidor.
1. **Criação do Objeto do Servidor**: É declarado e instanciando o objeto `CalculatorServer`.
1. **Inicialização do Servidor**: Através da API do DRb, é iniciado o servidor no URI definido.

### Cliente (`calc_client.rb`)

1. **Síntaxe de Execução**:
```
ruby calc_client.rb <ip_servidor>:<porta_servidor> <valor1> <operador> <valor2>
```
	* **ip_servidor**: IP registrado pelo servidor
	* **porta_servidor**: Porta registrada pelo servidor
	* **valor1, valor2**: valor numérico de ponto flutuante
	* **operador**: operador aritmético: `+`, `-`, `"*"`, `"**"` ou `/`
	* **Exemplo de Síntaxe**: `ruby calc_client.rb localhost:9000 3 "**" 3`
1. **Conexão com o Servidor**: Através da API do DRb, é recebido um objeto do servidor, de acordo com o IP e porta definidos pelo usuário.
1. **Validação das entradas**: É feito a validação dos valores e do operador de forma a garantir que a chamada para o servidor tenha os dados corretos.
1. **Pedido de cálculo**: Realiza-se a chamada do método `calculate()` contido no objeto do servidor com as entradas do usuário como parâmetro.
1. **Apresentação do resultado**: É informado ao usuário o resultado da operação realizada pelo servidor.

## Códigos

### `calc_client.rb`

```Ruby
require 'drb/drb'

class String
	def numeric?
		return true if self =~ /\A\d+\Z/
		true if Float(self) rescue false
	end
end

SERVER_URI = "druby://#{ARGV.first}"
server = DRbObject.new_with_uri(SERVER_URI)

if ARGV[1].numeric? then
	first_value = ARGV[1].to_f
	if first_value.respond_to?(ARGV[2]) then
		operator = ARGV[2]
		if ARGV[3].numeric? then
			second_value = ARGV[3].to_f
			answer = server.calculate(first_value, operator,
				second_value)
			puts "#{first_value} #{operator} #{second_value} =
				#{answer}"
		else
			raise "The second operand is not a number!"
		end
	else
		raise "Unknown operator!\nIf you want to multiply, insert
			the * with quotes:\nEx: 5 \'+\' 3"
	end
else
	raise "The first operand is not a number!"
end
```

-----

### `calc_server.rb`

```Ruby
require 'drb/drb'

class CalculatorServer
	def calculate(first_value, operator, second_value)
		puts "Request: #{first_value} #{operator} #{second_value}"
		answer = first_value.public_send(operator, second_value)
		puts "- Answer: #{answer}"
		return answer
	end
end

URI = "druby://#{ARGV.first}"
FRONT_OBJECT = CalculatorServer.new

puts 'Starting server...'
DRb.start_service(URI, FRONT_OBJECT)
puts "Listening on #{URI}"
DRb.thread.join
```

-----

## Considerações Finais

Dentre as duas ferramentas apresentadas, a ferramenta DRb da linguagem Ruby é a ferramenta de melhor facilidade de compreensão e implementação. A configuração das duas entidades é feita através de chamadas simples de funções do DRb, onde víncula um caminho de comunicação para o servidor.

A ferramenta RMI do Java possui dois complicadores:
1. A necessidade de se utilizar do serviço `rmiregistry` para gerenciar os registros de servidores RMI;
1. A necessidade de criar uma política de segurança de forma a garantir que a JVM permita a comunicação entre as entidades.

Devido ao fato da presente tarefa ter sido realizada individualmente, até o presente momento todas as execuções foram realizadas em apenas um _host_.

Mesmo que ambas as ferramentas utilizem da tecnologia de Invocação de Método Remoto (_Remote Method Invocation_, RMI) para estabelecer comunicação entre as entidades. Tais implementações não permitem a interação entre as partes sem a necessidade de uma entidade intermediária para converter os objetos e métodos para o contexto da outra linguagem. Dois casos de teste foram realizados de forma a validar essa afirmação:

### Caso de Teste 1: Servidor Ruby & Cliente Java

* **Terminal Servidor**: Criando Servidor
```
[1] $ ruby calc_server.rb localhost:8787
[2] Starting server...
[3] Listening on druby://localhost:8787
```
* **Terminal Cliente**: Tentativa de Comunicação 1
```
[1] $ java CalculatorClient localhost:8787 1 + 1
[2] Unknown hostname!
```
* **Terminal Servidor**: Sem mudanças
* **Terminal Cliente**: Tentativa de Comunicação 2
```
[3] $ java CalculatorClient druby://localhost:8787 1 + 1
[4] Unknown hostname!
```
* **Terminal Servidor**: Sem mudanças
* **Terminal Cliente**: Tentativa de Comunicação 3
```
[5] $ java CalculatorClient localhost 1 + 1
[6] Unknown hostname!
```
* **Terminal Servidor**: Sem mudanças

### Caso de Teste 2: Servidor Java & Cliente Ruby

* **Terminal Servidor**: Criando Servidor
```
[1] $ rmiregistry &
[2] $ make exec-server
[3] java -Djava.security.manager -Djava.security.policy=server.policy
[.] CalculatorServer
[4] Server ready.
```
* **Terminal Cliente**: Tentativa de Comunicação 1
```
[1] $ ruby calc_client.rb localhost 3 + 2
[2] /usr/lib/ruby/2.3.0/drb/drb.rb:827:in `parse_uri': can't
[.] parse uri:druby://localhost (DRb::DRbBadURI)
[3]	from /usr/lib/ruby/2.3.0/drb/drb.rb:893:in `uri_option'
[4]	from /usr/lib/ruby/2.3.0/drb/drb.rb:789:in `block in
[.] uri_option'
[5]	from /usr/lib/ruby/2.3.0/drb/drb.rb:787:in `each'
[6]	from /usr/lib/ruby/2.3.0/drb/drb.rb:787:in `uri_option'
[7]	from /usr/lib/ruby/2.3.0/drb/drb.rb:1099:in `initialize'
[8]	from /usr/lib/ruby/2.3.0/drb/drb.rb:1079:in `new'
[9]	from /usr/lib/ruby/2.3.0/drb/drb.rb:1079:in `new_with_uri'
[10]	from calc_client.rb:11:in `<main>'
```
* **Terminal Servidor**: Sem mudanças
* **Terminal Cliente**: Tentativa de Comunicação 2
```
[11] $ ruby calc_client.rb localhost:1099 3 + 2
[12] /usr/lib/ruby/2.3.0/drb/drb.rb:579:in `load': connection
[..] closed (DRb::DRbConnError)
[13]	from /usr/lib/ruby/2.3.0/drb/drb.rb:641:in `recv_reply'
[14]	from /usr/lib/ruby/2.3.0/drb/drb.rb:941:in `recv_reply'
[15]	from /usr/lib/ruby/2.3.0/drb/drb.rb:1254:in `send_message'
[16]	from /usr/lib/ruby/2.3.0/drb/drb.rb:1142:in `block
[..] (2 levels) in method_missing'
[17]	from /usr/lib/ruby/2.3.0/drb/drb.rb:1229:in `open'
[18]	from /usr/lib/ruby/2.3.0/drb/drb.rb:1141:in `block in method_missing'
[19]	from /usr/lib/ruby/2.3.0/drb/drb.rb:1160:in `with_friend'
[20]	from /usr/lib/ruby/2.3.0/drb/drb.rb:1140:in
[..] `method_missing'
[21]	from calc_client.rb:19:in `<main>'
```
* **Terminal Servidor**: Sem mudanças
* **Terminal Cliente**: Tentativa de Comunicação 3
```
[22] $ ruby calc_client.rb localhost:9001 3 + 2
[23] /usr/lib/ruby/2.3.0/drb/drb.rb:745:in `rescue in block in
[..] open': druby://localhost:9001 - #<Errno::ECONNREFUSED:
[..] Connection refused - connect(2) for "localhost" port 9001>
[..] (DRb::DRbConnError)
[24]	from /usr/lib/ruby/2.3.0/drb/drb.rb:739:in `block in open'
[25]	from /usr/lib/ruby/2.3.0/drb/drb.rb:738:in `each'
[26]	from /usr/lib/ruby/2.3.0/drb/drb.rb:738:in `open'
[27]	from /usr/lib/ruby/2.3.0/drb/drb.rb:1248:in `initialize'
[28]	from /usr/lib/ruby/2.3.0/drb/drb.rb:1228:in `new'
[29]	from /usr/lib/ruby/2.3.0/drb/drb.rb:1228:in `open'
[30]	from /usr/lib/ruby/2.3.0/drb/drb.rb:1141:in `block in
[..] method_missing'
[31]	from /usr/lib/ruby/2.3.0/drb/drb.rb:1160:in `with_friend'
[32]	from /usr/lib/ruby/2.3.0/drb/drb.rb:1140:in
[..] `method_missing'
[33]	from calc_client.rb:19:in `<main>'
```
* **Terminal Servidor**: Sem mudanças
