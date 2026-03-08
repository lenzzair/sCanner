#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>


int scan(char *ip, int port){

  int sock = socket(AF_INET, SOCK_STREAM, 0);

  if (sock == -1){
    perror("socket error");
    return 1;
  }

  struct sockaddr_in server;

  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr.s_addr = inet_addr(ip);


  if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
    return 1;
  }

  return 0;
  
  close(sock);

}


int main(int argn, char * argv[]){
  
  if (argn != 3){
    printf("Tools use: scan <ip> <port> \n");
    return 1;
  }

  char * ip_dst = argv[1];
  int port_start; int port_end;


  if (strcmp(argv[2], "all") == 0){

    port_start = 1;
    int port_end = 1204;

  } else {
    int port_end = atoi(argv[2]);
    int port_start = port_end;
  }
  

  // CREATION de la socket
  // Préparation de l'adresse du serveur

  for(int port = port_start; port >= port_end; port++){

    int result = scan(ip_dst, port);

    if (result == 0){
      printf("Port %d open \n", port);
    
    } else if (port_start == port_end){

      printf("Port %d close \n", port);

    }

  }

  // Connexion au serveur
  
  

  return 0;


}
