CC = gcc
CFLAGS = -Wall -Wextra

all: ex3q1 ex3q2a ex3q2b ex3q3a ex3q3b

ex3q1: Ex3q1.c
	$(CC) $(CFLAGS) -o ex3q1 Ex3q1.c

ex3q2a: Ex3q2a.c
	$(CC) $(CFLAGS) -o ex3q2a Ex3q2a.c -lrt -lpthread

ex3q2b: Ex3q2b.c
	$(CC) $(CFLAGS) -o ex3q2b Ex3q2b.c -lrt -lpthread

ex3q3a: Ex3q3a.c
	$(CC) $(CFLAGS) -o ex3q3a Ex3q3a.c -lrt -lpthread

ex3q3b: Ex3q3b.c
	$(CC) $(CFLAGS) -o ex3q3b Ex3q3b.c -lrt -lpthread

clean:
	rm -f ex3q1 ex3q2a ex3q2b ex3q3a ex3q3b
