

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
void *tr_handler(void *socket_desc)
{
    
    int sock = *(int *)socket_desc;
    int read_size;
    char *message, client_message[3000];

   
    message = "Greetings!\n"; // send  messages to the client
    write(sock, message, strlen(message));

   

    
    while ((read_size = recv(sock, client_message, 2000, 0)) > 0)//a message from client
    {
       
        write(sock, client_message, strlen(client_message));
    }

    if (read_size == 0)
    {
        puts("Client_disconnected");
        fflush(stdout);
    }
    else if (read_size == -1)
    {
        perror("recv_failed");
    }

    
    free(socket_desc);

    return 0;
}

int main(int argc, char *argv[])
{
    int socket_desc, client_sock, size, *new_sock;
    struct sockaddr_in server, client;

    socket_desc = socket(AF_INET, SOCK_STREAM, 0); // Create socket
    if (socket_desc == -1)
    {
        printf("not create socket");
    }
    puts("socket created");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8876);

    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) // bind
    {

        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

  
    listen(socket_desc, 3);  

    size= sizeof(struct sockaddr_in);

    
    puts("Waiting for  connections...");// Accept and incoming connection
    size= sizeof(struct sockaddr_in);
    while ((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&size)))
    {
        puts("accepted");

        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;

        if (pthread_create(&sniffer_thread, NULL, tr_handler, (void *)new_sock) < 0)
        {
            perror("not create thread");
            return 1;
        }
            puts("handler into server");
    }   
    if (client_sock < 0)
    {
        perror("failed");
        return 1;
    }
    
    return 0;
}



