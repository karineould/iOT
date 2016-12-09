CFLAG = -Wall 
CFLAG_R = -D_REENTRANT 
LFLAG = -pthread

default : clitcp stcp

stcp : stcp.o outils.o
	cc -o stcp stcp.o outils.o $(LFLAG)

clitcp : clitcp.o outils.o
	cc -o clitcp clitcp.o outils.o

stcp.o : stcp.c outils.h
	cc -c $(CFLAG) $(CFLAG_R) stcp.c

clitcp.o : clitcp.c outils.h
	cc -c $(CFLAG) clitcp.c

outils.o : outils.c outils.h
	cc -c $(CFLAG) $(CFLAG_R) outils.c

clean :
	rm -f stcp clitcp *.o


