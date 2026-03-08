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
    close(sock);
    return 1;
  }

  struct sockaddr_in server;

  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr.s_addr = inet_addr(ip);


  if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
    close(sock);
    return 1;
  }

  close(sock);
  return 0;
  

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
    port_end = 65535;

  } else {
    port_end = atoi(argv[2]);
    port_start = port_end;
  }

  printf("[+] Status IP : %s \n", ip_dst);
  printf("[+] Status port : %d - %d \n", port_start, port_end);
  printf("[*] Scanning starting... \n");

  for(int port = port_start; port <= port_end; port++){
    
    int result = scan(ip_dst, port);

    if (result == 0){
      printf("Port %d open \n", port);
    
    } else if (port_start == port_end){

      printf("Port %d close \n", port);

    }

  }
  

  return 0;


}
