# CS359 Mini-Project

This project is a TCP/IP stack that is run as a single daemon process on your Linux host. Networking is achieved by configuring your Linux host to forward packets to/from this application.

# Building

Standard `make` stuff.

    $ make all

This builds `cs359` itself.

When building, `sudo setcap ...` probably asks super user permissions from you. This is because `cs359` needs the `CAP_NET_ADMIN` capability to setup itself. After the setup, it drops that capability.

Currently, `cs359` also configures the tap interface through the `ip` tool. Hence, give it permissions too:

    $ which ip
    /sbin/ip

`/sbin/ip` being symbolic link to `/bin/ip` the next command is: 

    $ sudo setcap cap_net_admin=ep /bin/ip

# Setup

`cs359` uses a Linux TAP device to communicate to the outside world. In short, the tap device is initialized in the host Linux' networking stack, and `cs359` can then read the L2 frames:

    $ sudo mknod /dev/net/tap c 10 200
    $ sudo chmod 0666 /dev/net/tap

In essence, `cs359` operates as a host inside the tap device's subnet. 

# Testing

When you've built cs359 and setup your host stack to forward packets, to check if things are working properly:

    $ sudo ./cs359

Now, open a new terminal.
To test the ARP implementation for 1st Week's work, to see whether it replies to ARP requests correctly:

    $ apring -I tap0 10.0.0.4

Output should be like:
    
    $ ARPING 10.0.0.4 from 10.0.0.5 tap0
    $ Unicast reply from 10.0.0.4 [00:0C:29:6D:50:25]  0.569ms
    $ Unicast reply from 10.0.0.4 [00:0C:29:6D:50:25]  0.622ms

Now, try:

    $ arp

Output should have:
    $ Address                  HWtype  HWaddress           Flags Mask            Iface
    $ 10.0.0.4                 ether   00:0c:29:6d:50:25   C                     tap0

The kernel’s networking stack recognized the ARP reply from our custom networking stack, and consequently populated its ARP cache with the entry of our virtual network device. Success!

Now, to test 2nd week's work where we implemented a minimum viable IP layer and test it with ICMP echo requests:

    $ ping 10.0.0.4 
    
Example Output:

    $ Unknown IP header proto
    $ Unknown IP header proto
    $ ICMP V4 Echo request recieved!    
    $ ICMP V4 Echo request recieved!
    $ ICMP V4 Echo request recieved!
    $ ICMP V4 Echo request recieved!
    $ Unsupported ethertype 86dd

On the `cs359` running terminal you shall see "ICMP V4 Echo request recieved!" messages for each packet sent. ICMP V4 reply and IP transpission functions have not been implemented yet, so it ping command will show 0 packets recieved. 

