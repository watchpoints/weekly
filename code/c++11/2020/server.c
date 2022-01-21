#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>

#define	oops(m)		{perror(m); exit(1);}
int	main(){
	int	sock_id;
	struct	sockaddr_in	saddr;
	sock_id = socket(PF_INET, SOCK_STREAM, 0);
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port 	      = htons(9988);
	saddr.sin_family      = AF_INET;
	int ret = bind(sock_id, (struct sockaddr *) &saddr, sizeof(saddr));//绑定IP地址和端口
	if(ret == -1) oops("bind error");//如果返回-1，则绑定失败，一般为“Address alreay in use”
	int	i;
	for(i = 0; i < 6; i++){//连续创建六个子进程
		int pid = fork();
		if(pid == 0) break;
	}
	listen(sock_id, 1);
	while(1){
		int	sock = accept(sock_id, NULL, 0);
		char 	buf[128];
		int	readnum;
		readnum = read(sock, buf, 127);
		buf[readnum] = '\0';
		printf("pid=%d, mesg: %s\n", getpid(), buf);
		fflush(stdout);
		close(sock);
	}
	return 1;
}