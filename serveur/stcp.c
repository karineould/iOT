/* stcp.c : serveur TCP (mode connecté) */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>

#include "outils.h"

#define PORT 9999

struct sockaddr_in SAddr = { AF_INET }; /* le reste est a zero !*/

#define LBUF 512
char buf[LBUF];

char * add_ip (long A)
{
static char b[16];
    sprintf(b,"%u.%u.%u.%u", (unsigned int)((A>>24)&0xFF),
                             (unsigned int)((A>>16)&0xFF),
                             (unsigned int)((A>>8)&0xFF),
                             (unsigned int)(A&0xFF));
    return b; 
}

struct Elt {
   char nom[20];
   char ip[16];
} annuaire[] = {
  { "batman", "192.168.77.14" },
  { "spider", "192.168.88.12" },
  { "wonder", "192.168.99.82" },
  { "", "" }
};

/* code du service fourni par le serveur */
void service(int fd)
{
int i=0, n;
    /* on attend le message du client */
    if ((n = lireMess(fd,buf,LBUF,'\n')) == -1) 
        sprintf(buf,"Message trop long ! %d car maxi !\n",LBUF-1);
    else {
       /* recherche dans la table */
       while (strlen(annuaire[i].nom) > 0) {
          if (strcmp(buf,annuaire[i].nom) == 0) {
             sprintf(buf,"Reponse:%s\n",annuaire[i].ip);
             break;
          }
          i++;
       }
       if (strlen(annuaire[i].nom) == 0) sprintf(buf,"Non trouve !\n");
    }
    write(fd,buf,strlen(buf));
    close(fd);
}

int main(int N, char*P[])
{
int sid, cid;
socklen_t l;
struct sockaddr_in SRec; /* structure de reception des messages */

   /* on cree l'interface bi-directionnelle (un file descriptor) */
   if ((sid = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
      perror("socket"); return 1;
   }
   /* demande d'attachement a un port */
   /* si on veut choisir son port, on decommente la ligne suivante :
   SAddr.sin_port = htons(PORT);
   */
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
      service(cid);
   }
   close (sid);
   return 0;
}






