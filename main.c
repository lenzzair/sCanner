#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

typedef struct 
{
  int port;
  char * service;
} PortService;

PortService services[] = {

  {21, "FTP"},
  {22, "SSH"},
  {23, "TELNET"},
  {25, "SMTP"},
  {53, "DNS"},
  {80, "HTTP"},
  {443, "HTTPS"},
  {3306, "MySQL"},
  {8080, "HTTP-alt"},
  {8443, "HTTPS-alt"},
  {0,   NULL}
};


char *get_service(int port){
  // ==========
  // GET_SERVICE : 
  // fonction who permet to get the name of the service use with table above
  // ~~~~~~~~~~~~~~~~~~
  // param 1 => int port
  // return => name of the servie (HTTP)
  //        => unknown
  // ==========
  for (int i = 0; services[i].service != NULL; i++){
    if(services[i].port == port){
      return services[i].service;
    }
    
  }
  return "unknown";
}

int scan(char *ip, int port){
  // ==========
  // SCAN : 
  // fonction who start the network scan
  // ~~~~~~~~~~~~~~~~~~
  // param 1 => char ip, the target IP
  // param 2 => int port, the target dest port 
  // return => int 1, the port is close
  //        => int 0, the port is open
  // ==========
  int sock = socket(AF_INET, SOCK_STREAM, 0);

  if (sock == -1){
    perror("socket error");
    close(sock);
    return 1;
  }

  struct timeval timeout;
  timeout.tv_sec = 1;
  timeout.tv_usec = 0;
  setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
  


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
  int port_start; int port_end; int nb_port; int nb_port_open;


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
    char *string_port = get_service(port);
    if (result == 0){

      printf("Port : \t %d \t open \t %s \n", port, string_port);
      nb_port_open++;

    } else if (port_start == port_end){

      printf("Port %d close \n", port);

    }

    nb_port++;

  }

  printf("\n\n---\n %d port scan and %d open", nb_port, nb_port_open );
  

  return 0;


}
