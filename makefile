quash: main.o queue.o
	g++ -g -std=c++11 main.o tree.o queue.o -o quash

main.o: main.c command.o
	g++ -c -g -std=c++11 main.c

command.o: command.c
	g++ -c -g -std=c++11 command.c

clean:
	rm -f *~ *.o

