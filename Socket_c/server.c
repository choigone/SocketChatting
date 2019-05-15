#include "header.h"

#define BUF_SIZE 100
#define MAX_CLIENT 100

void * handle_client(void *arg);
void send_msg(char *msg, int len);
void handle_error(char *msg);
void menu();


int client_cnt=0;
int clnt_socks[MAX_CLIENT];
pthread_mutex_t mutx;

int main(int argc, char *argv[])
{
    int server_sock, client_sock;
    struct sockaddr_in server_address, client_address;
    int clnt_adr_sz;
    pthread_t t_id;

    struct tm *t;
    time_t timer = time(NULL);
    t=localtime(&timer);

    if (argc != 2)
    {
        printf(" Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    menu(argv[1]);

    pthread_mutex_init(&mutx, NULL);
    server_sock=socket(PF_INET, SOCK_STREAM, 0);

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family=AF_INET;
    server_address.sin_addr.s_addr=htonl(INADDR_ANY);
    server_address.sin_port=htons(atoi(argv[1]));

    if (bind(server_sock, (struct sockaddr*)&server_address, sizeof(server_address))==-1)
        handle_error("bind() error");
    if (listen(server_sock, 5)==-1)
        handle_error("listen() error");

    while(1){
        t=localtime(&timer);
        clnt_adr_sz=sizeof(client_address);
        client_sock=accept(server_sock, (struct sockaddr*)&client_address, &clnt_adr_sz);

        pthread_mutex_lock(&mutx);
        clnt_socks[client_cnt++]=client_sock;
        pthread_mutex_unlock(&mutx);

        pthread_create(&t_id, NULL, handle_client, (void*)&client_sock);
        pthread_detach(t_id);
        printf(" chatter (%d/100)\n", client_cnt);
    }
    close(server_sock);
    return 0;
}

void *handle_client(void *arg)
{
    int clnt_sock=*((int*)arg);
    int str_len=0, i;
    char msg[BUF_SIZE];

    while((str_len=read(clnt_sock, msg, sizeof(msg)))!=0){
        send_msg(msg, str_len);
    }

    pthread_mutex_lock(&mutx);
    for (i=0; i<client_cnt; i++){
        if (clnt_sock==clnt_socks[i]){
            while(i++<client_cnt-1)
                clnt_socks[i]=clnt_socks[i+1];
            break;
        }
    }
    client_cnt--;
    pthread_mutex_unlock(&mutx);
    close(clnt_sock);
    return NULL;
}

void send_msg(char* msg, int len)
{
    int i;
    pthread_mutex_lock(&mutx);
    for (i=0; i<client_cnt; i++){
        write(clnt_socks[i], msg, len);
    }
    pthread_mutex_unlock(&mutx);
}

void handle_error(char *msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}

void menu()
{
    system("clear");
    printf(" ***************************\n");
    printf(" ************Log************\n");
    printf(" ***************************\n\n");
}
