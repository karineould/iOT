/* boite a outil contenant nos fct personnelles */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "outils.h" /* pour verification !! */

int lireMess(int fd, char *b, int l, char eom)
/*   fd : file descriptor
     b  : adresse du buffer
     l  : longueur "   "
     eom: caractere de fin de message
**************************************/
{
char * d, *f;
   d=b;
   f=b+l;
   while (d < f) { /* lecture en d d'un octet */
      if (read (fd, (void*)d, 1) != 1) return -1;
      if (*d == eom) break;
      d++;
   }
   if (d==f) { /* le buffer est trop petit !! */
      /* on vide le reste du message incomplet */
      while (*b != eom) read(fd, (void*)b, 1);
      return -1;
   } else { /* on a trouve eom !! */
     *d = '\0'; /* pour etre compatible avec le standard des strings en C */
     return (d-b);
   }
}

/* fct qui affiche un message avec une entete contenant le PID et le PPID */
void Trace(char * m)
{
   fprintf(stderr,"PID:%d PPID:%d %s\n", getpid(), getppid(), m);
}


