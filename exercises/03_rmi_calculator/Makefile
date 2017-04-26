# Java Server/Client Makefile

# Parameters
CLIENT = CalculatorClient
SERVER = CalculatorServer
INTERFACE = Calculator
SERVER_POLICY = server.policy

BUILD = javac $(INTERFACE).java $(SERVER).java $(CLIENT).java

EXEC_SERVER = java -Djava.security.manager -Djava.security.policy=$(SERVER_POLICY) $(SERVER)

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
