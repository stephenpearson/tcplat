# TCP Latency
Simple tool to measure TCP handshake time between two nodes.
Use `make` to build.

```sh
Usage: ./tcplat <destination IP> <port> <attempts>
```
Example:
```sh
./tcplat 192.168.100.10 80 1000
TCP handshake latency: min=123us, mean=977us, max=5322us
```
