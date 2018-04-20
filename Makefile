OBJS = main.c

CC = gcc

calc: $(OBJS)
	$(CC) main.c -lpthread -o main

clean:
	rm main
