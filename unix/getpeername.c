#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char ** argv)
{
	int sockfd;
	struct sockaddr_in s;
	struct sockaddr sa;	
	struct sockaddr_in *sa_in = (struct sockaddr_in*)&sa;
	socklen_t sa_len = sizeof(struct sockaddr_in);	
	
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <ip> <port>\n", argv[0]);
		exit(1);
	}
	

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		fprintf(stderr, "Failed to create socket! (%s)\n", 
			strerror(errno));
		exit(1);
	}	

	memset(&s, 0x0, sizeof(s));
	
	s.sin_family = AF_INET;
	s.sin_port   = htons(atoi(argv[2]));

	if ( inet_aton(argv[1], &s.sin_addr) == 0) {
		fprintf(stderr, "%s is not a valid address\n", argv[1]);
		exit(2);
	}
	
	if (connect(sockfd, (const struct sockaddr*)&s, sizeof(s)) != 0) {
		fprintf(stderr, "Failed to connect! (%s)\n", strerror(errno));
		exit(2);
	}

	if (getpeername(sockfd, &sa, &sa_len) != 0) {
		fprintf(stderr, "getpeername failed! (%s)\n", strerror(errno));
	}	

	if ((sa_in->sin_family != AF_INET) || (sa_len != sizeof(struct sockaddr_in))) {
		fprintf(stderr, "WTF!\n");
	}	
		
	printf("Test suceeded.\n");
	
	close(sockfd);
	return 0;	
}
