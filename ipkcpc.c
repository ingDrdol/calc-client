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
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define ARG_NUM 6
#define WELL_KNOWN_PORTS 1023 //number of well known ports
#define MAX_PORT_NUM 65353 //max port number for tcp/udp protocols

extern int optopt, opterr, optind;
extern char *optarg;

void print_help(){
  printf("help is being printed\n");
}

int main(int argc, char** argv){
    int c, port_num, mode;
    char *host_name = NULL;
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
        host_name = malloc(sizeof(optarg));
        strcpy(host_name, optarg);
        break;
      case 'p':
        port_num = atoi(optarg);
        //////////////////////////////////////////////////////////////////////////////////////////////
        if(port_num <= WELL_KNOWN_PORTS || port_num >= MAX_PORT_NUM){
          fprintf(stderr, "ERROR: invalid port number '%d' must be within <1024, 65_353>\n", port_num);
          if(host_name != NULL)
            free(host_name);
          exit(1);
        }
        ////////////////////////////////^^kontrola cisla portu^^///////////////////////////////////////
        break;
      case 'm':
        if(!strcmp(optarg, "udp"))
          mode = 0;
        else if(!strcmp(optarg, "tcp"))
          mode = 1;
        else{
        /////////////////////////////////////////////////////////////////////////////////
          fprintf(stderr, "ERROR: unknown mode expected [udp tcp] given %s\n", optarg);
          if(host_name != NULL)
            free(host_name);
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

        free(host_name);
        exit(1);
      default:

        free(host_name);
        exit(1);
      }
    }
////////////////^^zpracovani argumentu^^/////////////////

///////////////////////////////////////
    if(optind < argc){
        fprintf(stderr, "Unexpected non-option arguments\n");
        if(host_name != NULL)
          free(host_name);
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
    
    /* tiskne informace o vzdalenem soketu */ 
    printf("INFO: Server socket: %s : %d \n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));
    if(mode)
      printf("tcp\n");
    else
      printf("udp\n");
    if(host_name != NULL)
      free(host_name);
    return 0;
}