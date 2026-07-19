# ft_malcom

`ft_malcom` is a C implementation of an ARP-based man-in-the-middle (MITM) tool for Linux. It operates directly on raw sockets to craft and send, ARP packets.

## Goals

- Perform ARP spoofing / cache poisoning between a source and target host to intercept traffic

## How it works

ARP (Address Resolution Protocol) maps IP addresses to MAC addresses on a local network. Hosts trust ARP replies without authentication, which makes it possible to "poison" a target's ARP cache by sending forged replies claiming that another host's IP address belongs to the attacker's MAC address.

1. It opens a raw `AF_PACKET` socket bound to `ETH_P_ARP`, so it receives ARP frames directly from the interface.
2. Incoming frames are parsed from the raw buffer into a structured `arp_pckt` representation.
3. Using the four required addresses (source IP/MAC, target IP/MAC), it crafts and sends spoofed ARP replies to assign the passed mac `source_mac` argument to the requested ip by the attacker.

## Build

```sh
make
```

## Usage

```sh
./ft_malcolm [options] <source_ip> <source_mac> <target_ip> <target_mac>
```

| Argument      | Description                                    |
|---------------|-------------------------------------------------|
| `source_ip`   | IP address of the first host to impersonate/target |
| `source_mac`  | MAC address of the first host                  |
| `target_ip`   | IP address of the second host                  |
| `target_mac`  | MAC address of the second host                 |
| `-v`          | Optional flag. Enables verbose logging of every step

Example:

```sh
sudo ./ft_malcolm 192.168.1.10 aa:bb:cc:dd:ee:01 192.168.1.20 aa:bb:cc:dd:ee:02 -v
```
you can pass hostanames, that will be resolved in the actual ip address.

## Requirements

- **OS**: Linux only (relies on Linux-specific `AF_PACKET`/`sockaddr_ll` raw socket API)
- **Dependencies**: none
- **Privileges**: raw sockets require elevated privileges. Run with `sudo` or grant the binary `CAP_NET_RAW`.

## Disclaimer

This tool performs ARP spoofing, which can disrupt network traffic and is illegal to use without authorization. It's intended strictly for learning about low-level networking and for use in controlled lab environments or networks you own / have explicit permission to test.