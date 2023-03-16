/*
*
* IPK projekt 1
*
* prijmeni, jmeno: Kocman, Vojta
* login: xkocma09
* soubor: ipkcpc.c
*/
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <strings.h>
#include <netdb.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define ARG_NUM 6
#define WELL_KNOWN_PORTS 1023 //number of well known ports
#define MAX_PORT_NUM 65353 //max port number for tcp/udp protocols
#define BUFF_SIZE 256
#define REQ_OFFSET 2 //pocet informacnich bajtu zpravy
#define REQ_OPCODE 0 //opcode pro request
#define REC_OFFSET 3 //pocet informacnich bajtu odpovedi
#define MAX_HOST_NAME_LEN 255

extern int optopt, opterr, optind;
extern char *optarg;
int csocket;

void print_help(){
  printf("help is being printed\n");
}

void send_udp(struct sockaddr_in address, int csocket, socklen_t socklen){
    char buffer[BUFF_SIZE];
    socklen_t recv_len = socklen;
    int status, opcode, chars;

    bzero(buffer, BUFF_SIZE);

    while(fgets(buffer + REQ_OFFSET, BUFF_SIZE - REQ_OFFSET, stdin)){

      recv_len = socklen;

      buffer[1] = strlen(buffer + REQ_OFFSET);
      buffer[0] = REQ_OPCODE;
      
      chars = sendto(csocket, buffer, strlen(buffer + REQ_OFFSET) + REQ_OFFSET, 0, (struct sockaddr *)&address, socklen);
      if(chars < 0){
        fprintf(stderr, "ERROR: unable to send reguest '%s'\n", buffer);
        exit(1);
      }

      bzero(buffer, BUFF_SIZE);
      chars = recvfrom(csocket, buffer, BUFF_SIZE, 0, (struct sockaddr *)&address, &recv_len);
      if(chars < 0){
        fprintf(stderr, "ERROR: response not given or lost\n");
        exit(1);
      }

      if(recv_len > socklen){
        fprintf(stderr, "Warning: response may not be whole\n");
      }

      opcode = (int)buffer[0];
      status = (int)buffer[1];
      recv_len = (int)buffer[2];

      if(opcode != 1){
        fprintf(stderr, "ERROR: unexpected opcode '%d'\n", opcode);
      }
      else if(status == 0){
        printf("OK:%s\n", buffer + REC_OFFSET);
      }
      else if(status == 1){
        printf("ERR:%s", buffer + REC_OFFSET);
      }
      else{
        fprintf(stderr, "ERROR: unexpected status\n");
      }

      bzero(buffer, BUFF_SIZE);
    }

}

void handle_sigint(int sig){
    fprintf(stderr, "Caught sigint, teminating connection\n");
    close(csocket);
    exit(2);
}

void send_tcp(struct sockaddr_in address, int csocket){

    char buffer[BUFF_SIZE];
    int chars;

    if(connect(csocket, (const struct sockaddr*) &address, sizeof(address))){
      fprintf(stderr, "ERROR: could not connect to server\n");
      exit(1);
    }

    bzero(buffer, BUFF_SIZE);
    while(fgets(buffer, BUFF_SIZE, stdin)){
      chars = send(csocket, buffer, strlen(buffer), 0);
      if(chars < 0){
        fprintf(stderr, "ERROR: could not send command %s\n", buffer);
      }

      chars = recv(csocket, buffer, BUFF_SIZE, 0);
      buffer[chars] = '\0';
      if(chars < 0)
        fprintf(stderr, "ERROR: didn't recive response\n");
      else
        printf("%s", buffer);
      
      if(strcmp(buffer, "BYE\n") == 0)
        break;
    }

    close(csocket);

}

int main(int argc, char** argv){
    int c, port_num = 0, mode = -1, sock_type = SOCK_DGRAM;
    char host_name[MAX_HOST_NAME_LEN];
    struct hostent *ipk_server;
    struct sockaddr_in server_addr;
//////////////////////////////////////////
    if(argc != (ARG_NUM + 1)){
      print_help();
      return 1;
    }
//////^^kontrola poctu argumentu^^////////

    opterr = 0;
/////////////////////////////////////////////////////////
    while ((c = getopt(argc, argv, "h:p:m:")) != -1){
    switch (c)
      {
      case 'h':
        strcpy(host_name, optarg);
        break;
      case 'p':
        port_num = atoi(optarg);
        //////////////////////////////////////////////////////////////////////////////////////////////
        if(port_num <= WELL_KNOWN_PORTS || port_num >= MAX_PORT_NUM){
          fprintf(stderr, "ERROR: invalid port number '%d' must be within <1024, 65_353>\n", port_num);
          exit(1);
        }
        ////////////////////////////////^^kontrola cisla portu^^///////////////////////////////////////
        break;
      case 'm':
        if(!strcmp(optarg, "udp")){
          mode = 0;
          sock_type = SOCK_DGRAM;
        }
        else if(!strcmp(optarg, "tcp")){
          signal(SIGINT, handle_sigint);
          mode = 1;
          sock_type = SOCK_STREAM;
        }
        else{
        /////////////////////////////////////////////////////////////////////////////////
          fprintf(stderr, "ERROR: unknown mode expected [udp tcp] given %s\n", optarg);
          exit(1);
        ////////////////////////////^^neznamy protokol^^//////////////////////////////////
        }
        break;
      case '?':
        if (optopt == 'h')
          fprintf (stderr, "Option -%c requires a hostname.\n", optopt);
        else if (optopt == 'p')
          fprintf (stderr, "Option -%c requires a port number.\n", optopt);
        else if (optopt == 'm')
          fprintf (stderr, "Option -%c requires a mode.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);

        exit(1);
      default:

        exit(1);
      }
    }
////////////////^^zpracovani argumentu^^/////////////////

///////////////////////////////////////
    if(optind < argc){
        fprintf(stderr, "Unexpected non-option arguments\n");
        exit(1);
    }
///^^kontrola necekanych argumentu^^///

////////////////////////////////////////////////////////////////////
    if ((ipk_server = gethostbyname(host_name)) == NULL) {
        fprintf(stderr,"ERROR: unknown hostname '%s'\n", host_name);
        exit(1);
    }
//////////^^ziskani adresy sereru pomoci DNS sluzby^^///////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *)ipk_server->h_addr_list[0], (char *)&server_addr.sin_addr.s_addr, ipk_server->h_length);
    server_addr.sin_port = htons(port_num);
/////////////////////////////^^nastaveni cisla potru a IP adresy hosta^^////////////////////////////////////    
    
	if ((csocket = socket(AF_INET, sock_type, 0)) <= 0){
		fprintf(stderr, "ERROR: socket not created\n");
    exit(1);
	}


    if(mode){
      send_tcp(server_addr, csocket);
    }
    else
      send_udp(server_addr, csocket, sizeof(server_addr));

    return 0;
}