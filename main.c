#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argn, char * argv[]){
  

  // CREATION de la socket
  int sock = socket(AF_INET, SOCK_STREAM, 0);

  if (sock == -1){
    perror("socket error");
    return 1;
  }

  // Préparation de l'adresse du serveur
  
  struct sockaddr_in server;

  server.sin_family = AF_INET;
  server.sin_port = htons(8000);
  server.sin_addr.s_addr = inet_addr("127.0.0.1");


  // Connexion au serveur
  
  if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
    perror("connect");
    return 1;

  }

  printf("Connect to the server");

  // Envoie du message 
  char message[] = "Hello bro";

  send(sock, message, strlen(message), 0);

  printf("message send");

  close(sock);

  return 0;


}
