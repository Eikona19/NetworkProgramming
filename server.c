#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#define BUF_SIZE 256
void DieWithError(char *errorMessage){
	perror(errorMessage);//標準エラー出力にエラーを返す
	exit(1);//エラーの時は1以上を返す
}
void commun (int sock){
	char buf[BUF_SIZE];
	int len_r; //受信文字数
	if((len_r = recv(sock,buf,BUF_SIZE,0))<=0)DieWithError("recv()failed");//受信文字数を返す
	buf[len_r]='\0'; //\0は文字列の終わりを示す
  	printf("%s\n",buf);
	if(send(sock,buf,strlen(buf),0)!=strlen(buf))DieWithError("send()sent a message of unexpected bytes");//送信文字数を返す
}
int main (int argc, char **argv){
	int cliSock;	
	int servSock = socket(PF_INET,SOCK_STREAM,0);
	struct sockaddr_in servAddress;
	unsigned int szClientAddr;
	struct sockaddr_in clientAddress;
	servAddress.sin_family = AF_INET;
	servAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddress.sin_port=htons(10001);
	
	bind(servSock,(struct sockaddr *)&servAddress,sizeof(servAddress));//structは構造体
	listen(servSock,5);//順番待ちしてもいいクライアント数
	
	while(1){
		szClientAddr = sizeof(clientAddress);
		cliSock = accept(servSock,(struct sockaddr *)&clientAddress,&szClientAddr);
		commun(cliSock);
	}
	close(servSock);
	return 0;
	
}