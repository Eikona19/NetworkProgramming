#include<sys/socket.h>
#include<stdio.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
void commun(int sock){
	int len_r;
	char buf[256];
	char*massage="福田";
	send(sock,massage,strlen(massage),0);
	len_r=recv(sock,buf,256,0);
	buf[len_r]='\0';
  	printf("%s\n",buf);
}
int main (int argc, char **argv){

	int sock = socket(PF_INET,SOCK_STREAM,0);
	
	//printf("%d\n",sock);
	struct sockaddr_in target;
	
	target.sin_family=AF_INET;
	target.sin_addr.s_addr=inet_addr("10.13.64.20");
	target.sin_port=htons(10001);
	connect(sock,(struct sockaddr*)&target,sizeof(target));
	
	commun(sock);
	
	close(sock);
	return 0;
}