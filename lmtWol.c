#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


#define MAGICSIZE 102
#define MAXBUF 65536

void usage()
{
	printf("lmtWol v0.1\n");
	printf("please enter the mac address and broadcast port in a following fashion ");
	printf("00:00:00:00:00:00 9\n"); 
}

int createMagicPacket(const char* inputMac, uint8_t* magicPacket)
{

	int readin[6];
	uint8_t macHex[6];
	memset(magicPacket, 0, MAGICSIZE);
	memset(macHex, 0, 6);
	int count = 6;
	//initial 6 full bytes
	for (int j = 0; j < 6; ++j)
	{
		magicPacket[j] = 255;
	}

	if( 6 == sscanf( inputMac, "%x:%x:%x:%x:%x:%x", &readin[0], &readin[1], &readin[2],&readin[3], &readin[4], &readin[5] ) )
	{
	    for(int i = 0; i < 6; ++i )
	        macHex[i] = (uint8_t) readin[i];
		for (int k = 0; k < 16; ++k){
			memcpy(&magicPacket[count], &macHex, 6);
			count = count +6;
		}
		return 1;
	}
	else return 0;
}

int main(int argc, char const *argv[])
{
	if (argc < 3){
		usage();
		exit(0);
	}
	uint8_t packet[MAGICSIZE];
	//==========
	int sock, status, buflen, sinlen;
	struct sockaddr_in sock_in;
	int yes = 1;
	
	sinlen = sizeof(struct sockaddr_in);
	memset(&sock_in, 0, sinlen);
	buflen = MAXBUF;
	
	sock = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sock_in.sin_addr.s_addr = htonl(INADDR_ANY);
	sock_in.sin_port = htons(0);
	sock_in.sin_family = PF_INET;
	
	status = bind(sock, (struct sockaddr *)&sock_in, sinlen);
	printf("Bind Status = %d\n", status);

	status = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(int) );
  	printf("Setsockopt Status = %d\n", status);

	if (createMagicPacket(argv[1], &packet[0]) == 1)
	{	
	sock_in.sin_addr.s_addr=htonl(-1); /* send message to 255.255.255.255 */
  	sock_in.sin_port = htons(atoi(argv[2])); /* port number */
  	sock_in.sin_family = PF_INET;
  	
  	status = sendto(sock, packet, MAGICSIZE, 0, (struct sockaddr *)&sock_in, sinlen);
	printf("sendto Status = %d\n", status);
  	
  	shutdown(sock, 2);
  	//close(sock);

  	}else
		 usage();
	//==========


	// const char* filename = "./somefile";
	// FILE* somefile;
	// somefile = fopen(filename, "w+");
	// fwrite(packet, sizeof(char), 102, somefile);
	// fclose(somefile);

	return 0;
}
