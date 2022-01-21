#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>

int	main(){
	int	i=0;
	struct	sockaddr_in	saddr;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port 	      = htons(9988);
	saddr.sin_family      = AF_INET;
	int	addlen 	          = sizeof(saddr);
	for(; i < 30; i++){
		int	sock=socket(AF_INET, SOCK_STREAM, 0);
		connect(sock, (struct sockaddr *) &saddr, addlen);
		write(sock, "Hello Server!", 13);
	}
}