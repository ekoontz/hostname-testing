Java and multiple network interfaces : (sun.net.spi.nameservice.provider.1 is your friend).
=========

While running Hadoop and HBase on my Macbook, I've often encountered
problems with hostname resolution. Like most laptop users, I have
multiple interfaces on my laptop, including the following:

* an ethernet port (en0)
* a WiFi interface (en1)
* a VirtualBox Host-only network (vboxnet0)

ifconfig output:

    $ ifconfig en1
    en1: flags=8863<UP,BROADCAST,SMART,RUNNING,SIMPLEX,MULTICAST> mtu 1500
    inet 192.168.5.223 netmask 0xfffffe00 broadcast 192.168.5.255

    $ ifconfig vboxnet0
    vboxnet0: flags=8843<UP,BROADCAST,RUNNING,SIMPLEX,MULTICAST> mtu 1500
	inet 192.168.56.1 netmask 0xffffff00 broadcast 192.168.56.255

When I start Hadoop-related services (namenodes, HBase regionservers,
etc), they end up listening on the wrong interface - it seems to be
the Wifi interface (en1) usually. Instead, I want them to listen on
vboxnet0. The latter is better because I can connect to these services
from my virtual machines, and also, the services are restricted to the
Host-only network, which is useful for security and for avoiding
physical network-related glitches (for example, moving my laptop from
one wireless network to another).

So, how to force Hadoop daemons to listen on vboxnet0? Using IntelliJ
to step through the hadoop-core unit test
`org.apache.hadoop.net.TestDNS` showed that Hadoop uses :

    InetAddress.getLocalHost().getCanonicalHostName() 

to determine what IP to use to listen on.

I then wrote a simple Java class "HostRelated.java"
(https://github.com/ekoontz/hostname-testing/blob/master/HostRelated.java)
to help understand what was going on with Java's InetAddress class.

Then, Googling for "java getLocalHost multiple interfaces"
(http://www.google.com/search?q=java+getLocalHost+multiple+interfaces)
 ended me up here: http://bugs.sun.com/bugdatabase/view_bug.do?bug_id=4665037 

Wherein I learned about the rather unintuitively-named system property
: `sun.net.spi.nameservice.provider.1`

You simply set this to `dns,sun` and your problems go away:

    $ java -Dsun.net.spi.nameservice.provider.1="dns,sun" HostRelated
    Java says your IP is: 192.168.56.1
    Java believes that your host's canonical hostname is : mac.foofers.org

Here's what happens when this system property is set to `sun`:

    $ java -Dsun.net.spi.nameservice.provider.1="sun" HostRelated
    Java says your IP is: 192.168.5.223
    Java believes that your host's canonical hostname is : 192.168.5.223

Based on the above evidence, I conclude that `sun` is the default value, but I
don't know that for sure.

Incidentally, the "mac.foofers.org" is from my own localhost-only DNS server:

    $ cat /etc/resolv.conf
    #...(snip)..
    nameserver 127.0.0.1
    # ..other (external) nameservers..
    
    $ dig @localhost mac.foofers.or
    
    ; <<>> DiG 9.6.0-APPLE-P2 <<>> @localhost mac.foofers.org
    ; (2 servers found)
    ;; global options: +cmd
    ;; Got answer:
    ;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 23718
    ;; flags: qr aa rd ra; QUERY: 1, ANSWER: 1, AUTHORITY: 1, ADDITIONAL: 0
    
    ;; QUESTION SECTION:
    ;mac.foofers.org.		IN	A
    
    ;; ANSWER SECTION:
    mac.foofers.org.	3600	IN	A	192.168.56.1
    
    ;; AUTHORITY SECTION:
    foofers.org.		3600	IN	NS	mac.foofers.org.
    
    ;; Query time: 0 msec
    ;; SERVER: 127.0.0.1#53(127.0.0.1)
    ;; WHEN: Sun Jun  5 08:12:35 2011
    ;; MSG SIZE  rcvd: 63

How to setup a VirtualBox host-only virtual network:

![networks](https://github.com/ekoontz/hostname-testing/blob/master/virtual%20box%20networks.png?raw=true)

![network config](https://github.com/ekoontz/hostname-testing/blob/master/configuring%20a%20host-only%20network.png?raw=true)




