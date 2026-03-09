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


void help(){

  printf("Usage: scan [-h] [-i] [-p 80]/[-r 21-1024]/[-a] [-v]\n");
  printf("-h \t Show this help");
  printf("-i \t Specifie a target IP\n");
  printf("\nYou can choose only one of this three options:\n\n");
  printf("-p \t Specifie a single port to scan\n");
  printf("-r \t Specifie a range of port to scan\n");
  printf("-a \t Scan all port (1-65535)\n");
  
}


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
  

  int verbose = 0;
  int port_start; int port_end; int nb_port; int nb_port_open;
  char *ip_dst;
  int opt;

  while ((opt = getopt(argn, argv, "hvai:p:r:")) != -1){
    switch (opt){
      case 'h': 
        help();
        return 0;

      case 'v':
        verbose = 1;
        break;
      
      case 'a':
        port_start = 1;
        port_end = 65535;
        break;  

      case 'i':
        ip_dst = optarg;
        break;
      case 'p':
        port_start = atoi(optarg);
        port_end = atoi(optarg);
        break;

      case 'r':
        port_start = atoi(strtok(optarg, "-"));
        port_end = atoi(strtok(NULL, "-"));
        break;

      case '?':
        printf("unknown option: %c \n", optarg);
        help();
        return 0;
    }
  }

  printf("[+] Status IP : %s \n", ip_dst);
  printf("[+] Status port : %d - %d \n", port_start, port_end);
  printf("[+] Status Verbose : %d \n", verbose);
  printf("[*] Scanning starting... \n");
  printf("\nPORT \t STATE \t SERVICE\n");

  for(int port = port_start; port <= port_end; port++){
    
    int result = scan(ip_dst, port);
    char *string_port = get_service(port);
    if (result == 0){

      printf("%d \t open \t %s \n", port, string_port);
      nb_port_open++;

    } else if (port_start == port_end){

      printf("%d \t close \t %s \n", port, string_port);

    }

    nb_port++;

  }

  printf("\n\n---\n %d port scan and %d open \n", nb_port, nb_port_open );
  

  return 0;


}
