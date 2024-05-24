all : run

run : build
	./snake

build : main.o clientAPI.o snakeAPI.o
	gcc -o snake main.o clientAPI.o snakeAPI.o

main.o : main.c
	gcc -o main.o -c main.c

clientAPI.o : clientAPI.c snakeAPI.c
	gcc -o clientAPI.o -c clientAPI.c

snakeAPI.o : snakeAPI.c
	gcc -o snakeAPI.o -c snakeAPI.c

clean :
	rm -f *.o
