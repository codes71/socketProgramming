#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 5000
#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
  

  const char* server_ip = "127.0.0.1";
  const char* message = "Hello from UDP";

  int sockfd;
  struct sockaddr_in serveraddr;

  // Create UDP socket
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    perror("socket creation failed");
    exit(1);
  }

  memset(&serveraddr, 0, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = inet_addr(server_ip);
  serveraddr.sin_port = htons(SERVER_PORT);

  // Send message to server
  int sent_bytes = sendto(sockfd, message, strlen(message), 0,
                         (struct sockaddr*)&serveraddr, sizeof(serveraddr));
  if (sent_bytes < 0) {
    perror("sendto failed");
    close(sockfd);
    exit(1);
  }

  printf("UDP: Sent message: %s\n", message);

  close(sockfd);

  return 0;
}
