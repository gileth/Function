#pragma once

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string>
#include <cstring>
#include <signal.h>
#include "MyDataBase.h"

using std::string;

#define LISTENQ 5
#define MAX_NUM 2

void SetUp(u_short port, int family, int type, string db, string tb);
void Socket(int protocal);
void Bind();
void Listen(int backlog);
int Accept(sockaddr_in &clientaddr);
void error_msg(string msg);
void Run();
void *Fun(void *arg);
void sig_act(int signo);
void Start(int protocal = 0, int backlog = 1024);
void first(int idx);