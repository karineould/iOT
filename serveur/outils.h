/* header file de notre boite a outil contenant nos fcts personnelles */

extern int lireMess(int fd, char *buf, int lbuf, char eom);
extern void Trace(char * message);
extern float memUsed(void);
extern float memFree(void);
extern int countProc(void);
extern float tauxCPU(void);


