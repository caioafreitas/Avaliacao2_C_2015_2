#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "socket.h"
#define MAX_SOCKETS 20

typedef struct
{
    char nome[50];
    SOCKET sock;
    int qtd_msg;
}S_sockets;

S_sockets sockets[MAX_SOCKETS];

int opened_sockets = 0;


int do_processing(S_sockets* sock);
SOCKET wait_connection(SOCKET* sockfd);

int msg_pvt(SOCKET* sockfd, S_sockets* socket, char* msg);

int main(int argc, char *argv[])
{
    SOCKET sock = create_socket();
    int i;
    for (i = 0; i < MAX_SOCKETS; ++i) {
        sockets[i].sock = INVALID_SOCKET;
        sockets[i].qtd_msg = 0;
    }


    if (sock != INVALID_SOCKET)
    {
        if (bind_socket(&sock, PORT_NO, 10) == 0)
        {
            while (1)
            {
                int idx, max;
                if (opened_sockets < MAX_SOCKETS)
                {
                    SOCKET new_sockfd = wait_connection(&sock);

                    if (new_sockfd != INVALID_SOCKET)
                    {
                        sockets[opened_sockets++].sock = new_sockfd;
                    }
                }

                for (idx = 0, max = opened_sockets; idx < max; idx++)
                {
                    if (sockets[idx].sock != INVALID_SOCKET)
                    {
                        if(do_processing(&sockets[idx])==-1)
                        {
                         printf("Disconnecting...\n");
                         disconnect_socket(&sockets[idx].sock);
                         sockets[idx].sock = sockets[opened_sockets-1].sock;
                         sockets[opened_sockets-1].sock = INVALID_SOCKET;
                         opened_sockets--;
                     }
                 }
             }
         }
     }
     else
     {
        perror("ERROR binding");
    }

    disconnect_socket(&sock);
}
else
{
    printf("ERROR opening socket");
}

destroy_socket(&sock);

return 0;
}


SOCKET wait_connection(SOCKET* sock)
{
    SOCKET new_sockfd = INVALID_SOCKET;

    if (*sock != INVALID_SOCKET)
    {
        int clilen;
        struct sockaddr_in cli_addr;

        listen(*sock, MAX_SOCKETS);
        clilen = sizeof(cli_addr);
        struct timeval tv = {0};
        tv.tv_usec = 100;

        FD_SET ReadSet={0};
        FD_SET(*sock, &ReadSet);
        if(select(0,&ReadSet,NULL,NULL,&tv)>0)
        {
            new_sockfd = accept(*sock, (struct sockaddr *) &cli_addr, (socklen_t*) &clilen);
            if (new_sockfd != INVALID_SOCKET)
            {
                printf("New connection incoming: %s:%d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
            }
        }  
    }

    return new_sockfd;
}

int do_processing(S_sockets* socket)
{
    int result = 0;

    if (socket->sock != INVALID_SOCKET)
    {
        int read;
        char buffer[256] = {0};

        FD_SET ReadSet={0};
        FD_SET(socket->sock, &ReadSet);
        struct timeval tv = {0};
        tv.tv_usec = 100;
        result = select(0,&ReadSet,NULL,NULL,&tv);

        if( result > 0){

            if ((read = recv_socket(&socket->sock, buffer, 255)) >= 0)
            {
                char msg_naoencontrado[] = {"Nick nao encontrado. Verifique digitacao."};
                int i,white=0,j=0;
                char msg[255]={0};
                char nick_destino[50]={0};
                
                if(socket->qtd_msg == 0)
                {
                    socket->qtd_msg++;
                    strcpy (socket->nome,buffer);
                    socket->nome[strlen(socket->nome)]='\0';
                    sprintf(msg, "Bem vindo %s", socket->nome);
                    printf("Conectou nick: %s\n",socket->nome);
                    send_socket(&socket->sock, msg, strlen(msg) );
                    return result;
                }

                switch(buffer[0])
                {

                    //Envia MSG privada
                    case '#':
                    for(i=0;i<strlen(buffer);i++)
                    {
                        if(white) msg[i-j]=buffer[i]; 
                        
                        if(white==0)
                        {
                            j++;
                            if(buffer[i]==' ') white++;
                            if(buffer[i]!='#' && buffer[i]!=' ') nick_destino[i-1]=buffer[i];
                        }
                    }

                    nick_destino[strlen(nick_destino)] = '\0';

                    for (i = 0; i < opened_sockets; ++i)
                    {
                        if(strcmp(sockets[i].nome,nick_destino) == 0)
                        {
                            char mensagem[255];
                            sprintf(mensagem, "@%s -> %s", socket->nome,msg);
                            send_socket( &sockets[i].sock, mensagem, strlen(mensagem) );
                            return result;
                        }
                    }

                    send_socket(&socket->sock, msg_naoencontrado, strlen(msg_naoencontrado) );
                    break;

                    //Retorna todos os NICKS
                    case '!':
                    for (i = 0; i < opened_sockets; ++i)
                    {
                        if(sockets[i].nome[0]!=0){
                            char mensagem[255]={0};
                            sprintf(mensagem, "#%s", sockets[i].nome);
                            send_socket(&socket->sock, mensagem, strlen(mensagem));
                        }   
                    }
                    break;
                    
                    //Envia msg para todos conectados
                    default:
                    printf("Broadcast\n");

                    memset((void*) msg, 0, 256);
                    for (i = 0; i < opened_sockets; ++i)
                    {
                        if(socket->sock!=sockets[i].sock)
                        {
                            printf("Nome: %s\n",socket->nome);                        
                            sprintf(msg, "[%s]-> %s", socket->nome,buffer);
                            printf("%s", msg);
                            send_socket(&sockets[i].sock, msg, strlen(msg));
                        }
                    }
                    break;
                }
               
                

            }
        }

    }

    return result;
}