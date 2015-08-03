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
