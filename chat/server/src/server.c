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

int main(int argc, char *argv[])
{
    SOCKET sock = create_socket();
    int i;
    for (i = 0; i < MAX_SOCKETS; ++i) 
	{
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
