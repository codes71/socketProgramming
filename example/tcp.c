#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
  if (argc < 3) {
    printf("Usage: %s <server_ip> <message>\n", argv[0]);
    exit(1);
  }

  const char* server_ip = argv[1];
  const char* message = argv[2];

  int sockfd;
  struct sockaddr_in serveraddr;

  // Create TCP socket
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("socket creation failed");
    exit(1);
  }

  memset(&serveraddr, 0, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = inet_addr(server_ip);
  serveraddr.sin_port = htons(SERVER_PORT);

  // Connect to server
  if (connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
    perror("connect failed");
    close(sockfd);
    exit(1);
  }

  // Send message to server
  int sent_bytes = send(sockfd, message, strlen(message), 0);
  if (sent_bytes < 0) {
    perror("send failed");
    close(sockfd);
    exit(1);
  }

  printf("TCP: Sent message: %s\n", message);

  char buffer[BUFFER_SIZE];
  int recv_bytes = recv(sockfd, buffer, BUFFER_SIZE, 0);
  if (recv_bytes > 0) {
    buffer[recv_bytes] = '\0';
    printf("TCP: Received response: %s\n", buffer);
  }

  close(sockfd);

  return 0;
}
