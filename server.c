#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    char buffer[BUFFER_SIZE];
    char resp[] = "HTTP/1.0 200 OK\r\n"
                  "Server: webserver-c\r\n"
                  "Content-type: text/html\r\n\r\n"
                  "<html>hello, world</html>\r\n";

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) 
    {
        perror("webserver (socket)");
        return 1;
    }
    printf("socket created successfully\r\n");

    struct sockaddr_in host_addr;
    int host_addr_len = sizeof(host_addr);

    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(PORT);
    host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr *)&host_addr, host_addr_len) != 0)
    {
        perror("webserver (bind)");
        return 1;
    }
    printf("socket successfully bound to address\r\n");

    if (listen(sockfd, SOMAXCONN) != 0) 
    {
        perror("webserver (listen)");
        return 1;
    }
    printf("server listening for connections\r\n");

    for (;;) 
    {
        int newsockfd = accept(sockfd,
                               (struct sockaddr *)&host_addr,
                               (socklen_t *)&host_addr_len);
        if (newsockfd < 0) 
        {
            perror("webserver (accept)");
            continue;
        }
        printf("connection accepted\r\n");

        int valread = read(newsockfd, buffer, BUFFER_SIZE);
        if (valread < 0)
        {
            perror("webserver (read)");
            continue;
        }

        int valwrite = write(newsockfd, resp, strlen(resp));
        if (valwrite < 0) 
        {
            perror("webserver (write)");
            continue;
        }

        close(newsockfd);
    }

    return 0;
}
