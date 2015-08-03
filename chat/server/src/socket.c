#include "socket.h"

SOCKET create_socket(void)
{
    
    SOCKET sock = INVALID_SOCKET;
    WSADATA wsaData = {0};

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) == 0)
    {
        sock = socket(AF_INET, SOCK_STREAM, 0);
    }

    return sock;
}

int bind_socket(SOCKET* sock, int port, int timeout_recv)
{
    int result = 0;

    if (*sock != INVALID_SOCKET)
    {
        struct sockaddr_in serv_addr = {0};

        struct timeval tv = {0};
        
        tv.tv_usec = 100;

        setsockopt(*sock, SOL_SOCKET, SO_RCVTIMEO, (char*) &tv, sizeof(struct timeval));

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(port);

        result = bind(*sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr));

        if (result == SOCKET_ERROR) {
            printf("bind failed with error %u\n", WSAGetLastError());
            destroy_socket(sock);
            WSACleanup();
            return 1;
        }
        printf("%d\n",result);
    }

    return result;
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

        if( result > 0)
		{

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

void disconnect_socket(SOCKET* sock)
{
 if (sock)
    {
        closesocket(*sock);
    }
}

void destroy_socket(SOCKET* sock)
{
   WSACleanup();
}

int send_socket(SOCKET* sock, char* buffer, int to_send)
{
    int sent = 0;

    if (sock && buffer && (to_send > 0))
    {
        sent = send(*sock, buffer, to_send, 0);
    }

    return sent;
}

int recv_socket(SOCKET* sock, char* buffer, int to_read)
{
    int read = 0;

    if (sock && buffer && (to_read > 0))
    {
        read = recv(*sock, buffer, to_read, 0);
    }

    return read;
}
