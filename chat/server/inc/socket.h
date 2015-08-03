#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PORT_NO 9090

SOCKET create_socket(void);
int bind_socket(SOCKET* sockfd, int port, int timeout_recv);
int do_processing(S_sockets* sock);
SOCKET wait_connection(SOCKET* sockfd);
void disconnect_socket(SOCKET* sock);
void destroy_socket(SOCKET* sock);
int send_socket(SOCKET* sock, char* buffer, int to_send);
int recv_socket(SOCKET* sock, char* buffer, int to_read);
