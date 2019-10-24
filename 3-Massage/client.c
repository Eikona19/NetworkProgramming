#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#define BUF_SIZE 256
#define MONEY_DIGIT_SIZE 10

void DieWithError(char *);

int prepare_client_socket(char *, int);

void my_scanf(char *, int);

void commun(int);

void read_until_delim(int, char *, char, int);

int main(int argc, char *argv[])
{

    if (argc != 3)

        DieWithError("usage: ./client ip_address port");

    int sock = prepare_client_socket(argv[1], atoi(argv[2]));

    commun(sock);

    close(sock);

    return 0;
}

void DieWithError(char *errorMessage)
{

    perror(errorMessage);

    exit(1);
}

int prepare_client_socket(char *ipaddr, int port)
{

    int sock = socket(PF_INET, SOCK_STREAM, 0);

    if (sock < 0)

        DieWithError("socket() failed");

    struct sockaddr_in target;

    target.sin_family = AF_INET;

    target.sin_addr.s_addr = inet_addr(ipaddr);

    target.sin_port = htons(port);

    if (connect(sock, (struct sockaddr *)&target, sizeof(target)) < 0)

        DieWithError("connect() failed");

    return sock;
}

void my_scanf(char *buf, int num_letter)
{
    //num_letterの文字だけ入力させる
    char format[20];
    sprintf(format, "%s%d%s", "%", num_letter, "s%*[^\n");
    scanf(format, buf);
    getchar();
}

void commun(int sock)
{
    char cmd[2] = "";
    char withdraw[MONEY_DIGIT_SIZE + 1]; //引き出し金額を入力
    char deposit[MONEY_DIGIT_SIZE + 1];  //預け入れ金額
    char meg[BUF_SIZE];

    printf("0:お引き出し　1:預け入れ　2:残高照会　9:終了\n");
    printf("何をしますか？＞");
    my_scanf(cmd, 1);
    switch (cmd[0])
    {
    case '0':
        //引き出し処理
        printf("引き出す金額を入力してください >");
        my_scanf(withdraw, MONEY_DIGIT_SIZE);

        sprintf(meg, "0_%s_", withdraw);
        break;
    case '1':
        printf("預け入れる金額を入力してください >");
        my_scanf(deposit, MONEY_DIGIT_SIZE);

        sprintf(meg, "%s_0_", deposit);
        break;
        //預け入れ処理
    case '2':
        //残高照会
        strcpy(meg, "0_0_");
        break;
    default:
        printf("ばーか\n");
        //unkownエラー
        return;
    }
    if (send(sock, meg, strlen(meg), 0) != strlen(meg))
        DieWithError("send( sena a message of unexpected bytes");
    //受信処理
    read_until_delim(sock, meg, '_', BUF_SIZE);
    //表示処理
    printf("残高%d円になりました", atoi(meg));
}
void read_until_delim(int sock, char *buf, char delimiter, int max_length)
{
    int len_r = 0;
    int index_letter = 0;
    while (index_letter < max_length - 1)
    {
        if ((len_r = recv(sock, buf + index_letter, 1, 0)) <= 0)
        {
            //エラー
            printf("接続が切れました\n");
            return;
        }
        if (buf[index_letter] == delimiter)
        {
            break;
        }
        else
        {
            index_letter++;
        }
    }
    //nullを末尾に追加
    buf[index_letter] = '\0';
}