/* stcp.c : serveur TCP (mode connecté) */
/* En multi-tache avec les threads Posix  */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

#include "outils.h"

#define PORT 9999

struct sockaddr_in SAddr = { AF_INET }; /* le reste est a zero !*/

#define LBUF 512

char * add_ip (long A)
{
static char b[16];
    sprintf(b,"%u.%u.%u.%u", (unsigned int)((A>>24)&0xFF),
                             (unsigned int)((A>>16)&0xFF),
                             (unsigned int)((A>>8)&0xFF),
                             (unsigned int)(A&0xFF));
    return b; 
}

void * newService(void * fd){

  char buff[LBUF];
  char buffMem[LBUF];
  int sock_err;

  //On verifie qu'on recoit bien les données du client 
  if ( sock_err = recv(fd, buff, sizeof(buff),0) < 0){
    fprintf(stderr, "receive failed\n\n");
    exit(1);
  }
  size_t nb_recv = (size_t) sock_err;

  //affiche le msg du client
  printf("buff reveive -> %s\n", buff);
  strncpy(buffMem, buff, strlen(buff));

  //vide le buffer
  buff[nb_recv] = 0;
  fflush(stdout);

  printf("buff after flush -> %s\n", buff);

  char type[3];
  strncpy(type, buff, 3);

      char *reply[50];
      int cpu = 1;
      int th = 2;
      

      printf("Start send data to %d.........\n", fd);
      while(1){
        cpu++;
        th++;
        sprintf(reply,"cpu=%d;thread=%d;", cpu, th); 
        //TODO verifier le send du client 
        sprintf(buff,"%s\n",reply);

        if (send(fd, buff, strlen(buff),0) != strlen(buff)){
          fprintf(stderr, "Send failed\n\n");
          // buff[nb_recv] = 0;
          // fflush(stdout);
          break;
        }

        printf("send -> %s\n", buff);
        sleep(1);
      }
      printf("fd close\n");
      close(fd);
}

int main(int N, char*P[])
{
int sid, cid, err;
pthread_t tid;
pthread_attr_t attr;
socklen_t l;
struct sockaddr_in SRec; /* structure de reception des messages */

  /* on initialise la gestion des signaux */
   signal(SIGPIPE, SIG_IGN);

   /* on cree l'interface bi-directionnelle (un file descriptor) */
   if ((sid = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
      perror("socket"); return 1;
   }
   /* demande d'attachement a un port */
   /* si on veut choisir son port, on decommente la ligne suivante :*/
   SAddr.sin_port = htons(PORT);
   
   if (bind(sid, (struct sockaddr *)&SAddr, sizeof(SAddr)) == -1) {
      perror("bind"); return 2;
   }
   /* on interroge le system pour connaitre le port */
   l = sizeof(SAddr);
   if (getsockname(sid, (struct sockaddr *)&SAddr, &l) == -1) {
      perror("getsockname"); return 3;
   }
   printf("Le serveur est attache au port %d.\n",ntohs(SAddr.sin_port));
   /* on dimensionne le "standard" ie le nb d'appels simultanes possibles */
   if (listen(sid, 5) == -1) {
      perror("listen"); return 4;
   }
   /* boucle de reception des messages */
   while (1) {
      if ((cid = accept(sid, (struct sockaddr *)&SRec, &l)) == -1 ) {
           perror("accept"); continue;
      }
      printf("Connexion avec %s\n", add_ip(ntohl(SRec.sin_addr.s_addr)));

      /* configuration de l'attribut pour que le thread soit LIBRE */
      pthread_attr_init(&attr);
      pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
      /* creation du thread charge de traiter la demande */
      if ((err= pthread_create(&tid, &attr, newService, (void*)((long)cid))) != 0)
        fprintf(stderr,"Erreur %d lors du pthread_create !\n",err);
   }
   close (sid);
   return 0;
}






