CFLAGS = -g #-lm

BINS = mega aposta bintests exp ulli sortear intptr

all: ${BINS}

mega.c: mega.h
mega: mega.c
#	cc mega.c -lm -o mega

aposta.c: mega.h
aposta: aposta.c

bintests: bintests.c
ulli: ulli.c
sortear: sortear.c
intptr: intptr.c

exp: exp.c
	cc exp.c -lm -o exp

test: ${BINS}
#	./bintests
	./mega

clean:
	rm -f ${BINS}
