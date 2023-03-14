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
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define COMP_ARG_NUM 6

extern int optopt, opterr, optind;

void print_help(){
  printf("help is being printed\n");
}

int main(int argc, char** argv){
    int c;
    opterr = 0;

    if(argc != (COMP_ARG_NUM + 1)){
      print_help();
      return 1;
    }

    while ((c = getopt(argc, argv, "h:p:m:")) != -1){
    switch (c)
      {
      case 'h':
        printf("host:\n");
        break;
      case 'p':
        printf("port:\n");
        break;
      case 'm':
        printf("mode:\n");
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
        return 1;
      default:
        return 1;
      }
    }
    
    if(optind < argc){
        fprintf(stderr, "Unexpected non-option arguments\n");
        return 1;
    }
    return 0;
}