# ft_malcom

`ft_malcom` is a C implementation of an ARP-based man-in-the-middle (MITM) tool for Linux. It operates directly on raw sockets to craft, send, and sniff ARP traffic, without relying on any external libraries.

## Goals

- Understand and manipulate the ARP protocol at the frame level
- Work directly with Linux raw sockets (`AF_PACKET`) instead of higher-level networking libraries
- Perform ARP spoofing / cache poisoning between a source and target host to intercept traffic
- Build a minimal, dependency-free networking tool in pure C

## How it works

ARP (Address Resolution Protocol) maps IP addresses to MAC addresses on a local network. Hosts trust ARP replies without authentication, which makes it possible to "poison" a target's ARP cache by sending forged replies claiming that another host's IP address belongs to the attacker's MAC address.

`ft_malcom` uses this to sit between a source and a target host:

1. It opens a raw `AF_PACKET` socket bound to `ETH_P_ARP`, so it receives ARP frames directly from the interface, bypassing the normal IP stack.
2. Incoming frames are parsed frame-by-frame from the raw buffer into a structured `arp_pckt` representation (`extract_arp_frame`), then printed for inspection (`print_arp_packet`).
3. Using the four required addresses (source IP/MAC, target IP/MAC), it crafts and sends spoofed ARP replies to redirect traffic between the two hosts through itself.
4. `sockaddr_ll` metadata (interface index, packet type, hardware address) is used to identify where each packet came from on the wire.

## Build

```sh
make
```

This compiles the sources and produces the `mitm` binary. The `Makefile` handles incremental rebuilds and cleanup targets (`clean`, `fclean`, `re`, adjust to match your actual targets).

## Usage

```sh
./mitm <source_ip> <source_mac> <target_ip> <target_mac> [-v]
```

| Argument      | Description                                    |
|---------------|-------------------------------------------------|
| `source_ip`   | IP address of the first host to impersonate/target |
| `source_mac`  | MAC address of the first host                  |
| `target_ip`   | IP address of the second host                  |
| `target_mac`  | MAC address of the second host                 |
| `-v`          | Optional flag. Enables verbose logging of every step (socket creation, packet reception, parsing) |

Example:

```sh
sudo ./mitm 192.168.1.10 aa:bb:cc:dd:ee:01 192.168.1.20 aa:bb:cc:dd:ee:02 -v
```

## Requirements

- **OS**: Linux only (relies on Linux-specific `AF_PACKET`/`sockaddr_ll` raw socket API)
- **Dependencies**: none — pure C, standard headers only (`sys/socket.h`, `linux/if_ether.h`, `netinet/in.h`, etc.)
- **Privileges**: raw sockets require elevated privileges. Run with `sudo` or grant the binary `CAP_NET_RAW`.

## Project structure

```
.
├── includes/
│   └── ft_malcom.h      # shared declarations (arp_pckt, log macros, prototypes)
├── src/
│   └── ...              # ARP parsing, packet handling, main loop
└── Makefile
```

## Logging

The project uses simple log helpers (`infolog`, `warnlog`, `errlog`) to report socket setup, packet reception, and errors — verbose (`-v`) mode surfaces more of this internal detail (e.g. raw packet type from `sockaddr_ll.sll_pkttype`).

## Disclaimer

This tool performs ARP spoofing, which can disrupt network traffic and is illegal to use without authorization. It's intended strictly for learning about low-level networking and for use in controlled lab environments or networks you own / have explicit permission to test.