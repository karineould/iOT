/* clitcp.c : client TCP (mode connecté) */
/* client adapte pour fonctionner avec le serveur factotum :
  il ne fait la connexion que quand il possede le nom a envoyer !!
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include "outils.h"

#define PORT 9999

#define LBUF 512
char buf[LBUF];

struct sockaddr_in SAddr = { AF_INET }; /* le reste est a zero !*/

int main(int N, char*P[])
{
int sid, n;
struct hostent *h;

   /* verifie les parametres */
   if (N != 3) {
      fprintf(stderr,"Utilisation : %s serveur port\n",P[0]); return 1;
   }
   /* on cree l'interface bi-directionnelle (un file descriptor) */
   if ((sid = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
      perror("socket"); return 2;
   }
   if ((h = gethostbyname(P[1])) == NULL) {
      perror("gethostbyname"); return 3;
   }
   /* on rempli SAddr avec l'adresse principale lue dans hostent */
   bcopy((void*)(h->h_addr), (void*)(&SAddr.sin_addr.s_addr), h->h_length);
   /* on rempli le port */
   SAddr.sin_port = htons(atoi(P[2]));
   /* on saisie le nom de la machine */
   printf("Taper le nom de la machine :\n");
   fgets(buf,LBUF,stdin);
   /* connexion avec le serveur */
   if (connect(sid,(struct sockaddr *)&SAddr, sizeof(SAddr)) == -1) {
      perror("connect"); return 4;
   }
   /* dialogue avec le serveur */
   /* on envoie le nom de la machine pour laquelle on veut l'IP */
   write(sid, buf, strlen(buf));
   /* on lit la reponse du serveur */
   if ((n = lireMess(sid, buf, LBUF, '\n')) == -1)
      printf("Le buffer est trop petit pour la réponse !\n");
   else printf("%s\n",buf);
   close (sid);
   return 0;
}






