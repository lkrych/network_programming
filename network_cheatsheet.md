# Network Cheatsheet

Inspired by Networking for System Administrators by Michael Lucas

All commands were run on Mac OSx

## Table of Contents
* [Ping](#ping)
* [Traceroute](#traceroute)
* [Arp](#arp)
* Network Interface
    * [Configuration](network-interface-configuration)
    * [ Statistics](#network-interface-statistics)
* Network Connections
    *

### Ping

The simplest command for checking IP connectivity between hosts. It requires an IP address or hostname you want to provoke a response from. Best for local network connectivity.

```bash
~/network_programming(master*) » ping 8a.nu                                         

PING 8a.nu (35.240.51.224): 56 data bytes
64 bytes from 35.240.51.224: icmp_seq=0 ttl=100 time=146.962 ms
64 bytes from 35.240.51.224: icmp_seq=1 ttl=100 time=148.203 ms
64 bytes from 35.240.51.224: icmp_seq=2 ttl=100 time=147.668 ms
^C

--- 8a.nu ping statistics ---
3 packets transmitted, 3 packets received, 0.0% packet loss
round-trip min/avg/max/stddev = 146.962/147.611/148.203/0.508 ms
```

### Traceroute

Another good tool for checking IP connectivity. This a good tool for checking connectivity to remote networks. It also requires an IP address or hostname.

```bash
~/network_programming(master*) » traceroute 8a.nu                                                                                    
traceroute to 8a.nu (35.240.51.224), 64 hops max, 52 byte packets
 1  sjc05-cp-mgre1-gw1-po17-1681.cisco.com (10.239.129.194)  8.113 ms  8.821 ms  8.163 ms
 2  sjc05-cp-mgre1-gw1-po18-1881.cisco.com (10.128.133.66)  8.350 ms  7.908 ms  7.880 ms
 3  171.69.7.177 (171.69.7.177)  8.437 ms  8.207 ms  8.346 ms
 4  sjc12-corp-gw1-ten1-3-0.cisco.com (171.69.7.138)  8.385 ms
    sjc05-corp-gw1-ten1-3-0.cisco.com (171.69.7.130)  8.083 ms  8.330 ms
 5  sjc12-dmzbb-gw1-vla777.cisco.com (128.107.225.205)  8.977 ms  8.836 ms  9.366 ms
 6  sjc12-cbb-gw1-be92.cisco.com (172.17.153.181)  9.938 ms  9.771 ms  9.660 ms
 7  sjc12-isp-gw2-ten1-0-0.cisco.com (128.107.224.253)  8.605 ms
    sjc12-isp-gw2-ten0-0-0.cisco.com (128.107.224.249)  8.920 ms  12.765 ms
 8  eqixsj-google-gige.google.com (206.223.116.21)  10.801 ms  9.903 ms  12.648 ms
 9  108.170.243.13 (108.170.243.13)  11.782 ms
    108.170.243.14 (108.170.243.14)  11.061 ms
    108.170.242.237 (108.170.242.237)  11.121 ms
10  142.250.234.55 (142.250.234.55)  11.224 ms
    72.14.237.160 (72.14.237.160)  10.684 ms
    142.250.234.55 (142.250.234.55)  11.455 ms
11  142.250.237.174 (142.250.237.174)  18.697 ms
    142.250.233.118 (142.250.233.118)  19.422 ms
    142.250.237.172 (142.250.237.172)  18.731 ms
12  142.250.235.174 (142.250.235.174)  51.793 ms  50.310 ms
    142.250.235.184 (142.250.235.184)  50.012 ms
13  142.250.232.127 (142.250.232.127)  60.415 ms  60.201 ms  60.676 ms
14  * 216.239.58.255 (216.239.58.255)  76.450 ms *
15  142.250.233.161 (142.250.233.161)  144.015 ms * *
16  209.85.252.5 (209.85.252.5)  149.026 ms  147.820 ms
    209.85.252.120 (209.85.252.120)  149.322 ms
17  108.170.231.203 (108.170.231.203)  151.786 ms
    108.170.231.215 (108.170.231.215)  149.453 ms
    216.239.56.27 (216.239.56.27)  148.729 ms
18  209.85.241.239 (209.85.241.239)  148.520 ms
    172.253.67.243 (172.253.67.243)  148.730 ms
    216.239.57.143 (216.239.57.143)  149.873 ms
19  * * *

```



### ARP

ARP maps ethernet addresses to IPv4 addresses and vice-versa. It is a common **glue between the datalink layer and the network layer**.

A host that needs to transmit to another host on an ethernet connection needs to find the MAC address for that other host. It does this by broadcasting an ARP request, "which MAC address corresponds to this IP address"? This goes to all hosts on the Ethernet network.

The host that owns that IP address responds with a request and the requesting host adds this MAC address to its ARP cache.

To view an arp cache, use `arp -a`.

```bash
~/network_programming(master*) » arp -a                                                                                lkrych@LKRYCH-M-W49D
? (192.168.1.1) at 30:5a:3a:70:fe:a0 on en0 permanent [ethernet]
```
**Neighbor discovery** is the protocol used to map IPv6 addresses, it is supposed to replace ARP. 

### Network Interface Configuration
`ifconfig`

```bash
~/network_programming(master*) » ifconfig                                                                            lkrych@LKRYCH-M-W49D
lo0: flags=8049<UP,LOOPBACK,RUNNING,MULTICAST> mtu 16384
	options=1203<RXCSUM,TXCSUM,TXSTATUS,SW_TIMESTAMP>
	inet 127.0.0.1 netmask 0xff000000
	inet6 ::1 prefixlen 128
	inet6 fe80::1%lo0 prefixlen 64 scopeid 0x1
	nd6 options=201<PERFORMNUD,DAD>
en0: flags=8863<UP,BROADCAST,SMART,RUNNING,SIMPLEX,MULTICAST> mtu 1500
	options=400<CHANNEL_IO>
	ether 8c:85:90:93:0e:df
	inet6 fe80::817:3e8b:3ffc:3843%en0 prefixlen 64 secured scopeid 0x5
	inet 192.168.1.75 netmask 0xffffff00 broadcast 192.168.1.255
	nd6 options=201<PERFORMNUD,DAD>
	media: autoselect
	status: active
...
```

### Network Interface Statistics
`netstat -i`

```bash
~/network_programming(master*) » netstat -i                                         

Name       Mtu   Network       Address            Ipkts Ierrs    Opkts Oerrs  Coll
lo0   16384 <Link#1>                       3388245     0  3388245     0     0
lo0   16384 127           localhost        3388245     -  3388245     -     -
lo0   16384 localhost   ::1                3388245     -  3388245     -     -
lo0   16384 fe80::1%lo0 fe80:1::1          3388245     -  3388245     -    
en0   1500  <Link#5>    8c:85:90:93:0e:df 42570767     0 32390764     0     0
en0   1500  fe80::817:3 fe80:5::817:3e8b: 42570767     - 32390764     -     -
en0   1500  192.168.1     192.168.1.75    42570767     - 32390764     -     -
...
```