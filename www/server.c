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
	char buf_old[BUF_SIZE];
	char buf2[2*BUF_SIZE];
	char response[BUF_SIZE];
	int len_r; //受信文字数
	buf_old[0]='\0' ;
	while((len_r = recv(sock,buf,BUF_SIZE,0))>0){
	buf[len_r]='\0'; //\0は文字列の終わりを示す
  	sprintf(buf2,"%s%s", buf_old, buf);
			if(strstr(buf2, "\r\n\r\n")) { //ifは成り立つか成り立たないかなので!=NULlがなくてもよい
				break;
			}
		sprintf(buf_old,"%s",buf);
	}
	if(len_r <= 0) DieWithError("received()filed.");	
	printf("recievd HTTP request.\n");
	if((len_r = recv(sock,buf,BUF_SIZE,0))<=0)
	sprintf(response,"HTTP/1.1 200 Ok \r\n");
	if(send(sock,buf,strlen(buf),0)!=strlen(response))DieWithError("send()sent a message of unexpected bytes");	
	sprintf(response,"Content-Type:text/html;charset=utf-8\n\r\n\r");
	if(send(sock,buf,strlen(buf),0)!=strlen(response))DieWithError("send()sent a message of unexpected bytes");
	sprintf(response,"<IDOCTYPE html><html><head><title>\r\n");
	if(send(sock,buf,strlen(buf),0)!=strlen(response))DieWithError("send()sent a message of unexpected bytes");
	sprintf(response,"ネットワークプログラミングのwebサイト\r\n");
	if(send(sock,buf,strlen(buf),0)!=strlen(response))DieWithError("send()sent a message of unexpected bytes");
	sprintf(response,"</title></head><body>ネットワークダイスキ</body></html>");
	if(send(sock,buf,strlen(buf),0)!=strlen(response))DieWithError("send()sent a message of unexpected bytes");
}
int main (int argc, char **argv){
	int cliSock;	
	int servSock = socket(PF_INET,SOCK_STREAM,0);
	struct sockaddr_in servAddress;
	unsigned int szClientAddr;
	struct sockaddr_in clientAddress;
	servAddress.sin_family = AF_INET;
	servAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddress.sin_port=htons(80);
	
	bind(servSock,(struct sockaddr *)&servAddress,sizeof(servAddress));//structは構造体
	listen(servSock,5);//順番待ちしてもいいクライアント数
	
	while(1){
		szClientAddr = sizeof(clientAddress);
		cliSock = accept(servSock,(struct sockaddr *)&clientAddress,&szClientAddr);
		commun(cliSock);
		close(cliSock);
	}
	close(servSock);
	return 0;
	
}