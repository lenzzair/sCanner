#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argn, char * argv[]){
 
  int port_dst = atoi(argv[1]);
  // CREATION de la socket
  int sock = socket(AF_INET, SOCK_STREAM, 0);

  if (sock == -1){
    perror("socket error");
    return 1;
  }

  // Préparation de l'adresse du serveur
  
  struct sockaddr_in server;

  server.sin_family = AF_INET;
  server.sin_port = htons(port_dst);
  server.sin_addr.s_addr = inet_addr("127.0.0.1");


  // Connexion au serveur
  
  if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
    printf("Port %d close \n", port_dst);
    return 1;
  }

  printf("Port %d open \n", port_dst);

  close(sock);

  return 0;


}
