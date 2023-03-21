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
#define MAX_HOST_NAME_LEN 255 //maximalni delka jmena domeny

///////////////////////////////////
extern int optopt, opterr, optind;
extern char *optarg;
/////^^promenne pro getopts^^//////

int csocket; //globalni kvuli uzavreni pri SIGINT

void print_help(){
  printf("help is being printed\n");
  /*
   * TO DO
   */
}
/*********************************************************************************************************
 * Title: Demonstration of trivial UDP communication
 * Author: Ondrej Rysavy (rysavy@fit.vutbr.cz)
 * Availability: https://git.fit.vutbr.cz/NESFIT/IPK-Projekty/src/branch/master/Stubs/cpp/DemoUdp/client.c
 ********************************************************************************************************/
void send_udp(struct sockaddr_in address, int csocket, socklen_t socklen){
    
    char buffer[BUFF_SIZE];
    socklen_t recv_len = socklen;
    int status, opcode, chars;

    bzero(buffer, BUFF_SIZE); //vynulovani bufferu

    while(fgets(buffer + REQ_OFFSET, BUFF_SIZE - REQ_OFFSET, stdin)){ //cyklus dokud fgets neprecne 0 znaku a nevrati NULL (vetsinou EOF)

      recv_len = socklen; //nastaveni promenne na puvodni delku soketu
      //////////////////////////////////////////
      buffer[1] = strlen(buffer + REQ_OFFSET);
      buffer[0] = REQ_OPCODE;
      //////^^nastaveni informacnich bitu^^/////
      chars = sendto(csocket, buffer, strlen(buffer + REQ_OFFSET) + REQ_OFFSET, 0, (struct sockaddr *)&address, socklen); //zaslani datagramu
      ///////////////////////////////////////////////////////////////////
      if(chars < 0){
        fprintf(stderr, "ERROR: unable to send reguest '%s'\n", buffer);
        exit(1);
      }
      ///////////////////^^kontrola odeslani datagramu^^/////////////////
      bzero(buffer, BUFF_SIZE);// vynulovani bufferu
			       // data nejsou vzdy ukoncena '\0'			    	
      chars = recvfrom(csocket, buffer, BUFF_SIZE, 0, (struct sockaddr *)&address, &recv_len); //prijeti datagramu
      ////////////////////////////////////////////////////////////////////////////////////////////
      if(chars < 0){
        fprintf(stderr, "ERROR: response not given or lost\n");
        exit(1);
      }

      if(recv_len > socklen){
        fprintf(stderr, "Warning: response may not be whole\n");
      }
      //////////////////////////////^^kontrola prijeti datagramu^^////////////////////////////////
      
      ///////////////////////////////
      opcode = (int)buffer[0];     //
      status = (int)buffer[1];     //
      recv_len = (int)buffer[2];   //
      ///////////////////////////////
      //^---- nastaveni informacnich bitu prijateho datagramu

      ////////////////////////////////////////////////////////////////
      if(opcode != 1){
        fprintf(stderr, "ERROR: unexpected opcode '%d'\n", opcode);
      }
      ////////////////^^kontrola spravnosti opcodu^^//////////////////
      else if(status == 0){
        printf("OK:%s\n", buffer + REC_OFFSET);
      }
      else if(status == 1){
        printf("ERR:%s\n", buffer + REC_OFFSET);
      }
      //////////////////^^tisk dat validni odpovedi^^/////////////////
      else{
        fprintf(stderr, "ERROR: unexpected status\n");
      }
      ///////////////////^^neznamy kod statusu^^//////////////////////

      bzero(buffer, BUFF_SIZE); //vynulovani bufferu
    }//end while

}//end send_udp

/*********************************************************************
 * Title: Example of sigint handler
 * Author: Kadam Patel
 * Date: 08. 02. 2018
 * Availibility: https://www.geeksforgeeks.org/signals-c-language/
 ********************************************************************/ 
///////////////////////////////////////////////////////////////////////////
void handle_sigint(int sig){                                       	 //
    int chars;							   	 //
    char buffer[BUFF_SIZE];					   	 //
    strcpy(buffer, "BYE\n");					   	 //
    printf( "%s", buffer);   						 //
    								  	 //
    while(chars = send(csocket, buffer, strlen(buffer),0) < 0);// 
    									 //
    chars = recv(csocket, buffer, BUFF_SIZE, 0);			 //
    buffer[chars] = '\0';						 //
    printf( "%s", buffer); 				                 //
    close(csocket);                                                      //
    exit(2);                                                       	 //
}                                                                  	 //
////////////////////^^zpracovani signalu 2 'SIGINT'^^//////////////////////

/*********************************************************************************************************
 * Title: Demonstration of trivial TCP communication
 * Author: Ondrej Rysavy (rysavy@fit.vutbr.cz)
 * Availability: https://git.fit.vutbr.cz/NESFIT/IPK-Projekty/src/branch/master/Stubs/cpp/DemoTcp/client.c
 ********************************************************************************************************/
void send_tcp(struct sockaddr_in address, int csocket){

    char buffer[BUFF_SIZE];
    int chars;
    //////////////////////////////////////////////////////////////////////////
    if(connect(csocket, (const struct sockaddr*) &address, sizeof(address))){
      fprintf(stderr, "ERROR: could not connect to server\n");
      exit(1);
    }
    ///////////////////////////^^otevreni spojeni^^///////////////////////////

    bzero(buffer, BUFF_SIZE); //vynulovani bufferu

    while(fgets(buffer, BUFF_SIZE, stdin)){ //cyklus dokud fgets neprecte 0 znaku a nevrati NULL (vetsinou EOF)
      chars = send(csocket, buffer, strlen(buffer), 0); //zaslani paketu

      /////////////////////////////////////////////////////////////////
      if(chars < 0){
        fprintf(stderr, "ERROR: could not send command %s\n", buffer);
      }
      //////////////////^^kontrola odeslani paketu^^///////////////////

      chars = recv(csocket, buffer, BUFF_SIZE, 0); //prijeti paketu
      buffer[chars] = '\0'; //nastaveni konce prijate zpravy
			    //zprava neni vzdy ukoncena '/0' a "zdvojene" ukonceni stringu nevadi ('/0/0')

      ///////////////////////////////////////////////////////
      if(chars < 0)
        fprintf(stderr, "ERROR: didn't recive response\n");
      /////////////^^chyba pri prijeti odpovedi^^////////////
      else
        printf("%s", buffer);
      ///////////////////^^tisk odpovedi^^///////////////////
      
      /////////////////////////////////////
      if(strcmp(buffer, "BYE\n") == 0)   //
        break;				 //
      /////////////////////////////////////
      //^------konec cyklu pri prijeti zpravy 'BYE'

    }//end while
    
    close(csocket); //uzavreni spojeni

}//end send_tcp

int main(int argc, char** argv){
    int c, port_num = 0, mode = -1, sock_type = SOCK_DGRAM;
    char host_name[MAX_HOST_NAME_LEN];
    struct hostent *ipk_server;
    struct sockaddr_in server_addr;
    //////////////////////////////////////
    if(argc != (ARG_NUM + 1)){
      print_help();
      return 1;
    }
    ////^^kontrola poctu argumentu^^//////

    opterr = 0;

/******************************************************************************************
 * Title: Example of getopt()
 * Author: unknown
 * Availability: https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
 *****************************************************************************************/
    /////////////////////////////////////////////////////
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
	///////////////////////////////////////////////////////////////////////
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
        
      default:
        exit(1);
        //////////////////^^neznamy, ci nevalidni prepinac^^///////////////////
      } // end switch
    } //end while
  //////////////^^zpracovani argumentu^^///////////////

    ////////////////////////////////////
    if(optind < argc){
        fprintf(stderr, "Unexpected non-option arguments\n");
        exit(1);
    }
   //^^kontrola necekanych argumentu^^//

    ////////////////////////////////////////////////////////////////
    if ((ipk_server = gethostbyname(host_name)) == NULL) {
        fprintf(stderr,"ERROR: unknown hostname '%s'\n", host_name);
        exit(1);
    }
    ////////^^ziskani adresy sereru pomoci DNS sluzby^^/////////////

    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *)ipk_server->h_addr_list[0], (char *)&server_addr.sin_addr.s_addr, ipk_server->h_length);
    server_addr.sin_port = htons(port_num);
    ///////////////////////////^^nastaveni cisla potru a IP adresy hosta^^/////////////////////////////////    
    
    //////////////////////////////////////////////////////
    if ((csocket = socket(AF_INET, sock_type, 0)) <= 0){
      fprintf(stderr, "ERROR: socket not created\n");
      exit(1);
    }
    //////////^^kontrola vytvoreni soketu^^///////////////

    //////////////////////////////////////////////////////
    if(mode){
      send_tcp(server_addr, csocket);
    }
    else
      send_udp(server_addr, csocket, sizeof(server_addr));
    //////////////^^vyber typu komunikace^^///////////////
    return 0;
}//end main
