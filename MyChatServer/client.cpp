#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <poll.h>

int main() {
	int clientfd;
	struct sockaddr_in clientaddr;
	char buff[1024];
	struct pollfd eventfd[2];
	clientfd = socket(AF_INET, SOCK_STREAM, 0);
	if (clientfd == -1) {
		printf("socket error\n");
		exit(0);
	}
	clientaddr.sin_family = AF_INET;
    clientaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientaddr.sin_port = htons(9999);
    if (connect(clientfd, (struct sockaddr *)&clientaddr, sizeof(clientaddr)) == -1) {
    	printf("connet error\n");
    	exit(0);
    }
    eventfd[0].fd = clientfd;
    eventfd[0].events = POLLRDNORM;
    eventfd[1].fd = fileno(stdin);
    eventfd[1].events = POLLRDNORM | POLLWRNORM;
    int nready;
    int stdineof = 0;
    int i;
    while (1) {
    	nready = poll(eventfd, 2, -1);

    	if (eventfd[0].revents & (POLLRDNORM | POLLERR)) {
    		if ( (i = read(clientfd, buff, 1024)) == 0 ) {
    			if (stdineof == 1)
    				break;
    			else {
    				printf("server closed!\n");
    				eventfd[0].fd = -1;
    				shutdown(clientfd, SHUT_WR);
    				break;
    			}
    		}
    		else {
    			printf("%s\n", buff);
    		}
		}
		
		if ( eventfd[1].revents & POLLRDNORM ) {
			scanf("%s", buff);
    		if (strcmp(buff, "OFF") == 0){
    			eventfd[1].fd = -1;
				stdineof = 1;
				shutdown(clientfd, SHUT_WR);
				continue;
			}
    		write(clientfd, buff, 1024);
    	}
    }
    close(clientfd);
    exit(0);
}