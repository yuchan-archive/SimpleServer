/**
 * Http Web Server
 *
 * @author Yusuke Ohashi
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014 Yusuke Ohashi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "util.h"

#define PORT 5555
#define MAXDATA 1024 

typedef struct {
    int port;
    char *mode;
} appconfig_t;

static appconfig_t defaultConf = {PORT,"hello"};

void parse_option(int argc, char**argv, appconfig_t **conf)
{
  if(*conf == NULL){
    exit(1);
  }
    int option;
    while((option = getopt(argc, argv, "ha:p:m:")) != -1){
        switch(option){
            case 'h':
                printf("Usage: ./hs [-p port] [-m mode]\n");
		free(*conf);
		exit(0);
                break;
            case 'p':
	      (*conf)->port = atoi(optarg);
                break;
            case 'm':
	      (*conf)->mode = optarg;
                break;
            case '?':
                printf("unexpected options.\n");
		free(*conf);
                exit(1);
                break;
        }
    }
}

int main(int argc, char **argv)
{
    appconfig_t *conf = (appconfig_t *)xmalloc(sizeof(appconfig_t *));
    conf = &defaultConf;
    parse_option(argc, argv, &conf);

    install_signal_handlers();
    struct sockaddr_in saddr;
    int port = conf->port;

    if((listen_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&saddr, 0, sizeof(saddr));

    saddr.sin_family = PF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(port);

    char opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));

    if(bind(listen_fd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0){
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if(listen(listen_fd, SOMAXCONN) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Starting Server, with Listening to port : %d \n", port);

    while(1){
        char buf[MAXDATA];
        struct sockaddr_in caddr;
        int conn_fd;
        socklen_t len;
        char method[MAXDATA];
        char url[MAXDATA];
        char protocol[MAXDATA];

        memset(&caddr, 0, sizeof(caddr));
        len = sizeof(caddr);
        if((conn_fd = accept(listen_fd, (struct sockaddr *)&caddr, &len)) < 0){
            perror("accept");
            exit(EXIT_FAILURE);
        }

        if(recv(conn_fd, buf, sizeof(buf), 0) < 0){
            perror("recv");
            exit(EXIT_FAILURE);
        }

        sscanf(buf, "%s %s %s", method, url, protocol);
        printf("%s", buf);

        char *header = "HTTP/1.0 200 OK\nContent-type: text/html\n\n";
        send(conn_fd, header, strlen(header), 0);
        char *hello = NULL;
        printf("%d",conf->port);
        if (strcmp(conf->mode,"hello")==0){
            hello = strdup("Hello World!\n");
        }else if(strcmp(conf->mode,"time") == 0){
	  char *buf = currentDate(&hello);
	  hello = (char *)xmalloc(sizeof(char *) * (strlen(buf) + 1));
	  hello = buf;
        }
        send(conn_fd, hello, strlen(hello), 0);
        free(hello);
        close(conn_fd);
    }
    close(listen_fd);
    free(conf);
    return 0;
}

