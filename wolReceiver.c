#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#define MAXBUF 65536

int main()
{
  int sock, status, buflen;
  unsigned sinlen;
  uint8_t buffer[MAXBUF];
  struct sockaddr_in sock_in;
  int yes = 1;

  sinlen = sizeof(struct sockaddr_in);
  memset(&sock_in, 0, sinlen);

  sock = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  sock_in.sin_addr.s_addr = htonl(INADDR_ANY);
  sock_in.sin_port = htons(7);
  sock_in.sin_family = PF_INET;

  status = bind(sock, (struct sockaddr *)&sock_in, sinlen);
  printf("Bind Status = %d\n", status);

  status = getsockname(sock, (struct sockaddr *)&sock_in, &sinlen);
  printf("Sock port %d\n",htons(sock_in.sin_port));
while (1){
  buflen = MAXBUF;
  memset(buffer, 0, buflen);
  status = recvfrom(sock, buffer, buflen, 0, (struct sockaddr *)&sock_in, &sinlen);
  printf("Received %d bytes\n", status);
  // for (int i = 0; i < status; ++i)
  // {
  //   printf("%.2X\n", buffer[i]);
  // }
  uint8_t mac[6];
  memset(mac,0,6);  
  for (int i = 6; i < 12; i++)
  {
    mac[i - 6] = buffer[i];
  }
if (mac[0] == 0xff)
  {
    shutdown(sock, 2);
    printf("Received exit code.\nExiting\n");
    exit(0);
  }else
  printf("Waking up mac %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n", mac[0],mac[1], mac[2],mac[3],mac[4],mac[5]);

}
  shutdown(sock, 2);
//  close(sock);
}
