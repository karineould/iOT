CFLAG = -Wall

default : clitcp stcp

stcp : stcp.o outils.o
	cc -o stcp stcp.o outils.o

clitcp : clitcp.o outils.o
	cc -o clitcp clitcp.o outils.o

stcp.o : stcp.c outils.h
	cc -c $(CFLAG) stcp.c

clitcp.o : clitcp.c outils.h
	cc -c $(CFLAG) clitcp.c

outils.o : outils.c outils.h
	cc -c $(CFLAG) outils.c

clean :
	rm -f stcp clitcp *.o


