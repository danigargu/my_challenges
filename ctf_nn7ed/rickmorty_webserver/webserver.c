/*
*
*   CTF Navaja Negra 2017
*   Description: RickMorty WebServer - stack-based overflow (ASLR/NX)
*   Author: @danigargu
*
*   $ gcc -m32 webserver.c -o webserver
*
*/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pwd.h>

#define LEVEL "rick" 
#define PORT 99
#define WEBROOT "./www"
#define DEFAULT_INDEX "index.html"

int create_server(void);
void handle_connection(int);
int get_file_size(int);
void sigchld_handler(int);
void send_bad_request(int);
void send_not_found(int);
void send_location(int, char *);
int recv_line(int, unsigned char *);
void send_string(int, char *);
void drop_privs(char *);

int main(void)
{
	int listen_fd, client_fd, pid;
	
	listen_fd = create_server();
	signal(SIGCHLD, SIG_IGN);
	printf("[*] Server listening on port: %d\n", PORT);

	while(1)
	{
		client_fd = accept(listen_fd, (struct sockaddr*)NULL, NULL);
		if(client_fd == -1){
			perror ("socket() ");
			exit(0);
		}
		if ((pid = fork()) == -1)
			perror("fork() ");
		else if (pid == 0)
		{
			drop_privs(LEVEL);
			handle_connection(client_fd);
			shutdown(client_fd, SHUT_RDWR);
			close(client_fd);
			exit(0);
		}
		close(client_fd);

	}
	return 0;
}

void sigchld_handler(int signal)
{
	wait(NULL);
}

int create_server(void)
{
	int listenfd = 0, yes = 1;
	struct sockaddr_in host_addr; // My address information

	if ((listenfd = socket(PF_INET, SOCK_STREAM, 0)) == -1){
		perror ("socket() ");
		exit(0);
	}

	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
		perror ("setsockopt() ");
		exit(0);
	}

	host_addr.sin_family = AF_INET;
	host_addr.sin_port = htons(PORT);
	host_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(host_addr.sin_zero), '\0', 8);

	if (bind(listenfd, (struct sockaddr *)&host_addr, sizeof(host_addr)) == -1){
		perror ("bind() ");
		exit(0);
	}

	if (listen(listenfd, SOMAXCONN) == -1){
		perror ("listen() ");
		exit(0);
	}
	return listenfd;
}


int is_directory(const char *path) 
{
	struct stat statbuf;
	if (stat(path, &statbuf) != 0)
		return 0;
	return S_ISDIR(statbuf.st_mode);
}

void handle_connection(int sockfd)
{
	unsigned char *ptr, request[512], resource[512];
	int fd, length, serve_file = 1;

	length = recv_line(sockfd, request); // OVERFLOW!! :D
	ptr = strstr(request, " HTTP/1.1"); // Search for valid-looking request.

	if (!ptr) 
	{
		send_bad_request(sockfd);
		return;
	}
	*ptr = 0;
	ptr = NULL;

	if (strncmp(request, "GET ", 4) == 0) // GET request
		ptr = request + 4;

	if (!ptr)
	{
		send_bad_request(sockfd);
		return;
	}
	strcpy(resource, WEBROOT);
	strcat(resource, ptr);

	if (strstr(resource, ".."))
	{
		send_bad_request(sockfd);
		return;
	}

	if (is_directory(resource))
	{
		if (resource[strlen(resource)-1] == '/')
			strcat(resource, DEFAULT_INDEX);
		else
		{
			serve_file = 0;
			strcat(ptr, "/");
			send_location(sockfd, ptr);
		}
	}
	if (serve_file)
	{
		fd = open(resource, O_RDONLY, 0);
		if (fd == -1)
		{
			send_not_found(sockfd);
		}
		else
		{
			send_string(sockfd, "HTTP/1.1 200 OK\r\n");
			send_string(sockfd, "Server: RickMorty/1.0b\r\n\r\n");

			if (ptr == request + 4)
			{
				if ((length = get_file_size(fd)) == -1)
				{
					perror ("get_file_size() ");
					exit(0);
				}
				if ((ptr = (unsigned char *) malloc(length)) == NULL)
				{
					perror ("malloc() ");
					exit(0);
				}
				read(fd, ptr, length);
				send(sockfd, ptr, length, 0); 
				free(ptr);
			}
			close(fd);
		}
	}	
}

void send_bad_request(int sockfd)
{
	send_string(sockfd, "HTTP/1.1 400 Bad Request\r\n");
	send_string(sockfd, "Server: RickMorty/1.0b\r\n\r\n");
	send_string(sockfd, "<h1>400 Bad Request</h1>\r\n");
}

void send_not_found(int sockfd)
{
	send_string(sockfd, "HTTP/1.1 404 Not Found\r\n");
	send_string(sockfd, "Server: RickMorty/1.0b\r\n\r\n");
	send_string(sockfd, "<html><head><title>404 Not Found</title></head>");
	send_string(sockfd, "<body><h1>404 Not Found</h1></body></html>\r\n");
}

void send_location(int sockfd, char *path)
{
	char r_location[256];
	snprintf(r_location, sizeof(r_location), "Location: %s\r\n", path); 

	send_string(sockfd, "HTTP/1.1 301 Moved Permanently\r\n");
	send_string(sockfd, r_location);
	send_string(sockfd, "Server: RickMorty/1.0b\r\n\r\n"); 
}

int get_file_size(int fd)
{
	struct stat stat_struct;
	if(fstat(fd, &stat_struct) == -1)
		return -1;
	return (int) stat_struct.st_size;
}

void send_string(int sockfd, char *buffer)
{
	send(sockfd, buffer, strlen(buffer), 0);
}

int recv_line(int sockfd, unsigned char *dest_buffer) 
{
	#define EOL "\r\n\r\n" // Bytes de fin de linea
	#define EOL_SIZE 4
	unsigned char *ptr;
	int eol_matched = 0;

	ptr = dest_buffer;

	while(recv(sockfd, ptr, 1, 0) == 1) {
		if(*ptr == EOL[eol_matched]) {
			eol_matched++;
			if(eol_matched == EOL_SIZE) {
				*(ptr+1-EOL_SIZE) = '\0';
				return strlen(dest_buffer);
			}
		} else {
			eol_matched = 0;
		}
		ptr++; 
	}
	return 0;
}

void drop_privs(char *username)
{
	struct passwd *pwd;
	if ((pwd = getpwnam(username)) == NULL)
		exit(1);
	
	if (setgroups(0, 0) < 0
	 || setresgid(pwd->pw_gid, pwd->pw_gid, pwd->pw_gid) < 0
	 || setresuid(pwd->pw_uid, pwd->pw_uid, pwd->pw_uid) < 0)
		exit(1);
}



