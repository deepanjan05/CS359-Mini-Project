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

In essence, `cs359` operates as a host inside the tap device's subnet. Therefore, in order to communicate with other hosts, the tap device needs to be set in a forwarding mode:

Determine the outgoing interface by using:
    
    $ route

An example from a Linux machine, where `wlp3s0` is the outgoing interface, and `tap0` is the tap device for `cs359`:

    $ sudo sysctl -w net.ipv4.ip_forward=1
    $ sudo iptables -I INPUT --source 10.0.0.0/24 -j ACCEPT
    $ sudo iptables -t nat -I POSTROUTING --out-interface wlp3s0 -j MASQUERADE
    $ sudo iptables -I FORWARD --in-interface wlp3s0 --out-interface tap0 -j ACCEPT
    $ sudo iptables -I FORWARD --in-interface tap0 --out-interface wlp3s0 -j ACCEPT


# Testing

When you've built cs359 and setup your host stack to forward packets, to check if things are working properly:

    $ sudo ./cs359

Now, open a new terminal.
Check weather `curl` is working or not:

    $ curl google.com 80

Output should be like: 

    <HTML><HEAD><meta http-equiv="content-type" content="text/html;charset=utf-8">
    <TITLE>301 Moved</TITLE></HEAD><BODY>
    <H1>301 Moved</H1>
    The document has moved
    <A HREF="http://www.google.com/">here</A>.
    </BODY></HTML>

Now `./tools/tool` is just a bash-script that allows `libcs359.so` to take precedence over the libc socket API calls.
The important point is that `./tool` aims to be usable against any existing dynamically-linked application. Hence we use the in-built `curl` to test out our tcp/ip stack and our implemented socket interface to communicate with application layer:

    $ ./tools/tool curl google.com 80

Output should be the same as normal curl if the programs are running properly.
Under the hood, however, `curl` calls the libc socket API but these calls are redirected to `./cs359` instead.