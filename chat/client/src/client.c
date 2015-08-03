#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <conio.h>
#include "socket.h"

int main(int argc, char *argv[])
{
	char serv_ip[20]={0}, nick[50]={0};
	int serv_port;
	
	printf ("IP do Servidor: ");
	scanf  ("%s", serv_ip);
	printf ("Porta do Servidor: ");
	scanf  ("%d", &serv_port);
	printf ("Digite seu nickname: ");
	scanf  ("%s", nick);
	
	SOCKET sock = create_socket();

    if (sock != INVALID_SOCKET)
    {
        if (connect_socket(&sock, serv_ip, serv_port, 10))
		{
			char buffer[256]={0};
			char msg[256]={0};
			int n=0;
			int i=0;
			char c;
			
			n = send_socket(&sock, nick, strlen(nick)); 	// Envia o nickname pro server
			if (n < 0)
                printf ("ERROR writing nickname to socket");
			
			memset((void*) buffer, 0, 256);
			memset((void*) msg, 0, 256);
			
			printf ("\n\n--> Para mensagem privada, digite #nick_destinatario e a mensagem \n--> Para Broadcast, digite a mensagem \n--> Para consultar os usuarios existentes digite !\n");
			printf ("Entrada: ");
			
			while (1)
			{
				if(kbhit())
				{
					c = getch();
					if(c == 8)
						msg[--i] = ' ';
					else
						msg[i++] = c;
					printf("%c",c);	
				}
				
				if (c == 13)
				{
					printf("\n");
					c = 0;
					msg[i] = '\0';
					n = send_socket(&sock, msg, strlen(msg));
					i = 0;
					memset((void*) msg, 0, 256);
					if (n < 0)
					printf ("ERROR writing to socket");
				}
				
				if (recv_socket(&sock, buffer, 255))
				{
					if (strlen(buffer) > 0)
					{	
						printf ("\n%s\n", buffer);
						memset((void*) buffer, 0, 256);
					}
				}	
			}					
		}
		else
        {
            printf ("ERROR connecting");
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
