#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 5000    
#define BUFFER_SIZE 1024

void handle_tcp_client(int sockfd) {
  char buffer[BUFFER_SIZE];
  int recv_bytes;

  // Receive message from client
  recv_bytes = recv(sockfd, buffer, BUFFER_SIZE, 0);
  if (recv_bytes < 0) {
    perror("recv failed");
    close(sockfd);
    return;
  }

  buffer[recv_bytes] = '\0';
  printf("TCP: Received message: %s\n", buffer);

  // Send response to client
  const char* response = "Hello from the TCP server!";
  send(sockfd, response, strlen(response), 0);

  close(sockfd);
}

void handle_udp_client(int sockfd, struct sockaddr_in* client_addr) {
  char buffer[BUFFER_SIZE];
  socklen_t client_len = sizeof(*client_addr);
  int recv_bytes;

  // Receive message from client
  recv_bytes = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                        (struct sockaddr*)client_addr, &client_len);
  if (recv_bytes < 0) {
    perror("recvfrom failed");
    close(sockfd);
    return;
  }

  buffer[recv_bytes] = '\0';
  printf("UDP: Received message: %s\n", buffer);

  // No traditional response for UDP, but can echo the message back
  sendto(sockfd, buffer, recv_bytes, 0,
        (struct sockaddr*)client_addr, client_len);
}

int main() {
  int sockfd, new_sockfd;
  struct sockaddr_in serveraddr, clientaddr;
  socklen_t clientlen;

  // Create TCP/IP socket
  sockfd = socket(AF_INET, SOCK_STREAM | SOCK_DGRAM, 0); // Supports both TCP and UDP
  if (sockfd < 0) {
    perror("socket creation failed");
    exit(1);
  }

  memset(&serveraddr, 0, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = INADDR_ANY;
  serveraddr.sin_port = htons(SERVER_PORT);

  // Bind the socket to the address
  if (bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
    perror("bind failed");
    close(sockfd);
    exit(1);
  }

  // Listen for incoming connections (TCP only)
  if (listen(sockfd, 5) < 0) { // Queue up to 5 connections
    perror("listen failed");
    close(sockfd);
    exit(1);
  }
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
  printf("Server listening on port %d (TCP and UDP)\n", SERVER_PORT);

  while (1) {
    // Accept connections (TCP)
    clientlen = sizeof(clientaddr);
    new_sockfd = accept(sockfd, (struct sockaddr*)&clientaddr, &clientlen);
    if (new_sockfd < 0) {
      perror("accept failed");
      continue;
    }

    int is_tcp = (getsockopt(new_sockfd, IPPROTO_TCP, SO_TYPE, NULL, 0) == 0);

    if (is_tcp) {
      printf("TCP client connected from %s:%d\n",
             inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
      handle_tcp_client(new_sockfd);
    } else {
      printf("UDP client connected from %s:%d\n",
             inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
      handle_udp_client(new_sockfd, &clientaddr);
    }

    close(new_sockfd);
  }

  close(sockfd);
  return 0;
}