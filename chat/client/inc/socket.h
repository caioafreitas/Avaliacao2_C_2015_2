#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define IP_ADDR "192.0.0.0"
#define PORT_NO 9090

SOCKET create_socket (void);
int connect_socket( SOCKET* sock, char* ip, int port, int timeout_recv);
void disconnect_socket (SOCKET* sock);
void destroy_socket (SOCKET* sock);
int send_socket (SOCKET* sock, char* buffer, int to_send);
int recv_socket (SOCKET* sock, char* buffer, int to_read);
int keyboard_read (char* message, char* buffer, int to_read);