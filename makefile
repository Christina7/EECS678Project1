<<<<<<< HEAD
quash: main.c
        gcc -g main.c -o quash
=======
quash: main.o queue.o
	gcc -g -c main.o -o quash

main.o: main.c command.o
	gcc -g -c main.c

command.o: command.c
	gcc -g -c command.c

>>>>>>> 4543deff100c0715a7707ba8f1eb43b4312a88bd
clean:
        rm -f *~ *.o


