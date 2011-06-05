.PHONY: testjava

testjava: HostRelated.class
	java -Dsun.net.spi.nameservice.provider.1="sun" HostRelated
	java -Dsun.net.spi.nameservice.provider.1="dns,sun" HostRelated

gethostbyname: 
	gcc gethostbyname.c -o gethostbyname

HostRelated.class:
	javac HostRelated.java




