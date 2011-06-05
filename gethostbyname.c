#include <netdb.h>
#include <stdio.h>
#include <arpa/inet.h>

int h_errno;

int main() {
  //   struct hostent *myhost = gethostbyname("mac.foofers.org");
   struct hostent *myhost = gethostbyname("mac.foofers.org");

   printf("HOSTENT INFO:\n");
   printf("  h_name: %s\n",myhost->h_name);
   printf("  h_aliases:(size: %ld)\n",(sizeof(myhost->h_aliases)/sizeof(char*)) - 1);

   char* alias = *(myhost->h_aliases);
   while(alias) {
     printf("    h_alias:(%s)\n",alias);
     alias++;
   }
   
   printf("  h_addrtype: %d",myhost->h_addrtype);
   if (myhost->h_addrtype == AF_INET) {
     printf(" (internet address)");
   }
   printf("\n");

   printf("  h_length: %d\n",myhost->h_length);

   if (myhost->h_addr_list) {
     int i;
     for (i = 0; myhost->h_addr_list[i]; i++) {
       struct in_addr **pptr;
       pptr = (struct in_addr **)myhost->h_addr_list;

       while( *pptr != NULL ){
	 printf("  address: %s\n",inet_ntoa(**(pptr++)));
       }

     }
   }

   return 0;

}
