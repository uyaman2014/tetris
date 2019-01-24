teto: kbhit.o teto.o
	gcc -o teto kbhit.o teto.o

kbhit.o: kbhit.h kbhit.c
	gcc -c kbhit.c

main.o: teto.c
	gcc -c teto.c

clean:
	rm -f *.o
