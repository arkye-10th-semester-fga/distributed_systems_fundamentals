03/09/2017 - Thursday

# Middleware

*	Communication, nomination, processes, synchronization, consistency, replication...
* The OS only see the middleware, not the other systems.

# Network Revision

## IP Addresses

* Reserved:
 	* `0.0.0.0` - Default gateway
	* `127.0.0.0` - Loopback address
	* `3.255.255.255` - Broadcast address

| IP Class | 1st Byte | 2nd Byte | 3rd Byte | 4th Byte |
|:--------:|:--------:|:--------:|:--------:|:--------:|
| A | 0 (7 bits) Server | Host | Host | Host |
| B | 10 (6 bits) Server | (8 bits) Server | Server | Server |
| C | 110 (5 bits) Server | (8 bits) Server | (8 bits) Server | Host |
| D | Multicast | | | |
| E | Reserved | | | |

* Mask: `255.255.255.0` -> `Server.Server.Server.Host` (Class C Mask)

* Command to distribute address:

```Shell
$ ifconfig eth0 192.168.1.2 netmask 255.255.255.0 broadcast 129.168.1.255
```

* Command to fill the route table:

```Shell
$ route add -net 192.168.1.0 netmask 255.255.255.0 eth0
# -> eth0: 192.168.1.2
$ route add -net 192.168.1.0 netmask 255.255.255.0 gw 172.25.0.1
# -> When it is necessary to post into other computer (172.25.0.1) in order to go the network (192.168.1.0)
$ cat /proc/sys/net/ipv4/ip_forward
# -> 0: Don't repost packages
# -> 1: Repost packages
```

* List the route table:

```Shell
$ netstat -nr
```

* Turn off `network.manager`

* Delete a line in the route table:

```Shell
$ route del -net 172.25.0.0 netmask 255.255.255.0
```

* Accessing other machine in the network:

```Shell
$ adduser name
$ ssh name@172.25.0.2
```

* Daemon: Process without console
	* Runs in an infinite loop, but don't interact with the user. Wait requisitions that in general comes from the network
	* Each daemon hears from one port
	* Each tab in the browser is a port

* Quintuple that allows the communication/identification in connectivity:
	* Origin port
	* Origin IP
	* Destination port
	* Destination IP
	* Transport type

* SOCK_DORAM - Socket UDP
* SOCK_T - Socket TCP

```Shell
$ scp -r user@172.25.0.2:/home/user/dsf_experiment
# -> Copy with ssh
```

* Test the codes (TCP/UDP) with loopback address 127.0.0.1:5000
