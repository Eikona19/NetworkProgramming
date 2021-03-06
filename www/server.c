#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<regex.h>
#define BUF_SIZE 256
void DieWithError(char *errorMessage){
	perror(errorMessage);//標準エラー出力にエラーを返す
	exit(1);//エラーの時は1以上を返す
}
void commun (int sock){
	regex_t regBuf;
	regmatch_t regMatch[1];
	char buf[BUF_SIZE];
	char buf_old[BUF_SIZE];
	char buf2[2*BUF_SIZE];
	char response[BUF_SIZE];
	int len_r; //受信文字数
	char result[100];
	char *uri;
	const char *pattern = "GET[^\n]+HTTP";
	if(regcomp(&regBuf,pattern,REG_EXTENDED | REG_NEWLINE)!=0){
		DieWithError("regcomp failed");
	}
	result[0]='\0';
	buf_old[0]='\0' ;
	while((len_r = recv(sock,buf,BUF_SIZE,0))>0){
		if(regexec(&regBuf,buf2,1,regMatch,0)!=0){
			int startIndex = regMatch[0].rm_so;
			int endIndex = regMatch[0].rm_eo;
			strncpy(result,buf2+startIndex,endIndex-startIndex); //cpy = copy
		}
	buf[len_r]='\0'; //\0は文字列の終わりを示す
  	sprintf(buf2,"%s%s", buf_old, buf);
			if(strstr(buf2, "\r\n\r\n")) { //ifは成り立つか成り立たないかなので!=NULlがなくてもよい
				break;
			}
		sprintf(buf_old,"%s",buf);
	}
	regfree(&regBuf);
	if(result[0]!='\0'){
		uri = strtok(uri," ");
		uri = strtok(NULL," ");
	}else{
		DieWithError("No URI");
	}
	if(len_r <= 0) DieWithError("received()filed.");	
	printf("recievd HTTP request.\n");
	sprintf(response, "HTTP/1.1 200 OK\r\n");
    if(send(sock, response, strlen(response), 0) != strlen(response))
        DieWithError("send() sent a message of unexpected bytes");
    
    sprintf(response, "Content-Type: text/html; charset=utf-8\r\n");
    if(send(sock, response, strlen(response), 0) != strlen(response))
        DieWithError("send() sent a message of unexpected bytes");
        
    sprintf(response, "\r\n");
    if(send(sock, response, strlen(response), 0) != strlen(response))
        DieWithError("send() sent a message of unexpected bytes");
    
    sprintf(response, "<!DOCTYPE html><html><head><title>");
    if(send(sock, response, strlen(response), 0) != strlen(response))
        DieWithError("send() sent a message of unexpected bytes");
    
    sprintf(response, "ネットワークプログラミングのwebサイト");
    if(send(sock, response, strlen(response), 0) != strlen(response))
        DieWithError("send() sent a message of unexpected bytes");
    
    sprintf(response, "</title></head><body>ネットワークダイスキ</body></html>");
    if(send(sock, response, strlen(response), 0) != strlen(response))
        DieWithError("send() sent a message of unexpected bytes");
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