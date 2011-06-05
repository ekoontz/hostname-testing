import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.UnknownHostException;

/*

$ java -Dsun.net.spi.nameservice.provider.1="sun" HostRelated
Java says your IP is: 192.168.5.223
Java believes that your host's canonical hostname is : 192.168.5.223

$ java -Dsun.net.spi.nameservice.provider.1="dns,sun" HostRelated
Java says your IP is: 192.168.56.1
Java believes that your host's canonical hostname is : mac.foofers.org

*/


class HostRelated {


    public static void main(String[] args) {
        String hostname;
        InetAddress hostAddress;
        try {
            hostAddress = InetAddress.getLocalHost();
            hostname = hostAddress.getCanonicalHostName();
            System.out.println("Java says your IP is: " + hostAddress.getHostAddress());
            System.out.println("Java believes that your host's canonical hostname is : " + hostname);
        }
        catch (UnknownHostException e) {
            System.err.println(e);
        }

    }
}