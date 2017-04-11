11/04/2017 - Terça-Feira

# Revisão

* Comunicação:
	* Transiente: MPI, Sockets
	* Persistente: MQSeries, Serviços de E-mail

# LDAP

## Serviços de Diretório

* Nomes
	* Espaço de nomes
	* Localização
* `cat /usr/sbin/arq.txt`
* Serviço de nomes DNS
	* .
		* com
		* edu
		* br
	* `www.unb.br`
	* Pergunta/Resposta

* Agendas de correio eletrônico
	* Precisam localizar o endereço eletrônico do usuário a partir do nome
* Serviços para cadastro de contas de usuário
	* Precisam localizar as informações de senha a partir do nome do usuário

* Serviços de diretório possuem esquema de localização mais sofisticado
	* SGBD-Read-Only
* A existência de múltiplos serviços de diretório cria dificuldades admnistrativas e incompatibilidade entre as aplicações
* X500 é um conjunto de padrões definido pelo CCITT
	* Banco de informações distribuído
	* Mecanismo de procura de informações flexível
	* Estrutura OO
		* RDN: Relative Distinguished Name
			* Exemplo: `cn=jdasilva`
	* Estrutura em Árvore
		* DN: Distinguished Name
			* Exemplo: `cn=jdasilva,ou=ppgia,o=pucpr.br`
	* Modelo funcional
		* DUA (Directory User Agent) -> DAP (Directory Access Protocol) -> DSP (Directory Service Protocol) -> DSA (Directory System Agent)
	* LDAP: implementação TCP/IP do X.500 (Lightweight DAP)
		* Autenticação
		* Contatos
		* Serviço de E-mails
		* NDS - Novell
		* AP - Microsoft
		* Lotus Notes/Domino
		* Cliente LDAP -> Servidor LDAP
		* Síntaxe de uma consulta LDAP:
			* Escopo sub: Sub-Árvore
			* Escopo one: Filhos imediatos
			* Escopo base: Nó
			* `ldap[s]://<host>:<porta>/<base_dn>?<atributos>?<escopo>?<filtro>`
				* Ex: `ldap://ppgia.pucpr.br/o=pucpr,c=br?email?sub?(sn=Joa*)`
				* Ex: `ldap://pucpr.br/ou=ppgia,o=pucpr.br/`
					* Aponta para o objeto ppgia na pucpr
				* Ex: `ldap://pucpr.br/ou=ppgia,o=pucpr.br/??sub?`
					* Aponta para todos os objetos abaixo do domínio ppgia
		* Filtros LDAP
	* SAMBA:
		* 1ª Parte:
			* Base LDAP
				* Schema (Classes)
				* Objetos
			* `$ ldapsearch`
			* `$ ldapadd`
			* `$ ldapremove`
		* 2ª Parte:
			* Serviço de Autenticação
			* Base AD (X500) LDAP -> SAMBA
			* Windows <-> SAMBA <-> Base LDAP
