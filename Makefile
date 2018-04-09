OBJS = main.c

CC = gcc

calc: $(OBJS)
	$(CC) main.c -ll -o main

clean:
	rm main