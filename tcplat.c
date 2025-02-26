#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <limits.h>

int main(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr, "Usage: %s <destination IP> <port> <attempts>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char *target_ip = argv[1];
  int target_port = atoi(argv[2]);
  int attempts = atoi(argv[3]);

  if (attempts <= 0) {
    fprintf(stderr, "Attempts must be a positive integer.\n");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server;
  struct timeval start, end;
  long min_time = LONG_MAX, max_time = 0, total_time = 0;

  server.sin_family = AF_INET;
  server.sin_port = htons(target_port);
  if (inet_pton(AF_INET, target_ip, & server.sin_addr) <= 0) {
    perror("inet_pton");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < attempts; i++) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
      perror("socket");
      exit(EXIT_FAILURE);
    }

    gettimeofday(&start, NULL);
    if (connect(sock, (struct sockaddr * ) & server, sizeof(server)) < 0) {
      perror("connect");
      close(sock);
      exit(EXIT_FAILURE);
    }
    gettimeofday(&end, NULL);
    close(sock);

    long elapsed = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    if (elapsed < min_time) min_time = elapsed;
    if (elapsed > max_time) max_time = elapsed;
    total_time += elapsed;
  }

  printf("TCP handshake latency: min=%ldus, mean=%ldus, max=%ldus\n", min_time, total_time / attempts, max_time);
  return 0;
}
