/* boite a outil contenant nos fct personnelles */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "outils.h" /* pour verification !! */

#define LBUF 128

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

static int lireVal(char * chemin, char * premot, long * T, int n)
{
FILE *fd;
char buf[LBUF], *d,*f;
int i;
    if ((fd = fopen(chemin,"r")) == NULL) {
       perror(chemin); return -1;
    }
    /* selection de la ligne */
    while (fgets(buf,LBUF,fd) != NULL) {
      /*  test du 1er mot */
      if (strncmp(buf,premot,strlen(premot)) != 0) continue;
      /* on decoupe l'info pour ranger les valeurs dans le tableau */
      d=buf+strlen(premot);
      while (*d == ' ') d++; /* on arrive au debut de la 1ere val */
      for (i=0; i<n; i++) {
          f=d;
          while (*f != ' ') f++; /* on arrive a la fin de la val */
          *f='\0';
          T[i]=atol(d);
          d=f+1;
      }
      return 0;
    }
    return -1;
}

float tauxCPU(void)
{
long T1[4], T2[4],N=0,D;
float Taux=-1.;
int i;
    if (lireVal("/proc/stat","cpu ", T1, 4) == -1) return Taux;
    sleep(1);
    if (lireVal("/proc/stat","cpu ", T2, 4) == -1) return Taux;
    for (i=0;i<3;i++) N+= (T2[i] - T1[i]); 
    D = N + T2[3] - T1[3];
    Taux = (float)(N*100L) / (float)D;
    return Taux;
}

//memoire utilisé
float memUsed(void)
{
  char command[4096];
  char line[256];
  float mem = -1.0;

  strcpy(command, "free | awk 'FNR == 3 {print $3/($3+$4)*100}'");
  FILE *file = popen(command, "r");

  while (fgets(line, sizeof(line), file) != NULL) 
  {       
    mem = atof(line);

  }
  return mem;
}

//memoire libre
float memFree(void)
{
  char command[4096];
  char line[256];
  float mem = -1.0;

  strcpy(command, "free | awk 'FNR == 3 {print $4/($3+$4)*100}'");
  FILE *file = popen(command, "r");

  while (fgets(line, sizeof(line), file) != NULL) 
  {       
    mem = atof(line);

  }
  return mem;
}

//count processus
int countProc(void)
{
  char command[4096];
  char line[256];
  int mem = -1;

  strcpy(command, "ps aux | wc -l");
  FILE *file = popen(command, "r");

  while (fgets(line, sizeof(line), file) != NULL) 
  {      
    mem = atoi(line);
  }
  return mem;
}

/* fct qui affiche un message avec une entete contenant le PID et le PPID */
void Trace(char * m)
{
   fprintf(stderr,"PID:%d PPID:%d %s\n", getpid(), getppid(), m);
}


