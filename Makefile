CFLAGS = -Wall -Wextra -pedantic -ggdb

executable: main.o
	$(CC) $(CFLAGS) ./main.o -o app
	rm -f *.o

main.o: funcs.o
	$(CC) $(CFLAGS) -c ./main.c -o main.o

funcs.o:
	$(CC) $(CFLAGS) -c ./funcs.c -o funcs.o