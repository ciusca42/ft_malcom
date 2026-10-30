# virtual switch, lives in the host namespace
sudo ip link add br0 type bridge
sudo ip addr add 10.0.0.1/24 dev br0
sudo ip link set br0 up

# two fake machines, each wired to the switch
for n in a b; do
  sudo ip netns add $n
  sudo ip link add veth-$n type veth peer name eth0 netns $n
  sudo ip link set veth-$n master br0
  sudo ip link set veth-$n up
  sudo ip netns exec $n ip link set lo up
  sudo ip netns exec $n ip link set eth0 up
done

sudo ip netns exec a ip addr add 10.0.0.2/24 dev eth0
sudo ip netns exec b ip addr add 10.0.0.3/24 dev eth0

# turn of automatic arp reply
sudo ip netns exec a sysctl -w net.ipv4.conf.eth0.arp_ignore=8

# test from b to a
sudo ip nens exec b ping -c 1 10.0.0.2

# check arp table
sudo ip netns exec b arp