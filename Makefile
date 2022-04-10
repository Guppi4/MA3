TARGET=client server
CC=gcc
CFLAGS= -Wall -Wextra -g
normal: $(TARGET)
client: client.c
	$(CC) $(CFLAGS) client.c -o client -lpthread
server: server.c
	$(CC) $(CFLAGS) server.c -o server -lpthread
clean:
	$(RM) $(TARGET)		
