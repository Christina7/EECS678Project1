quash: main.o queue.o
	gcc -g -c main.o -o quash

main.o: main.c command.o
	gcc -g -c main.c

command.o: command.c
	gcc -g -c command.c

clean:
	rm -f *~ *.o

