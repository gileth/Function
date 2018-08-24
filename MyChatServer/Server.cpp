#include "Server.h"

static pthread_t tid[MAX_NUM];
static int fd[MAX_NUM];
static sockaddr_in sock[MAX_NUM];
static string clientname[MAX_NUM];
static pthread_mutex_t mv = PTHREAD_MUTEX_INITIALIZER;
static sockaddr_in serveraddr;
static int connect_type;
static int serverfd;
static MyDataBase mdb;
static string tablename;

void SetUp(u_short port, int family, int type, string db, string tb) {
	bzero(&serveraddr, sizeof(serveraddr));
	
	connect_type = type;
	serveraddr.sin_family = family;
	serveraddr.sin_port = htons(port);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	mdb.connect("localhost", "root", "1234");
	mdb.createdb(db);
	mdb.usedb(db);
	mdb.createtb(tb, "name varchar(255), password varchar(255) default \"000000\"");
	mdb.selectitem(tb, "*");
	tablename = tb;
}

void Socket(int protocal) {
	serverfd = socket(serveraddr.sin_family, connect_type, protocal);
	if (serverfd == -1) {
		error_msg("socket error!");
	}
}

void Bind() {
	if (bind(serverfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1) {
		error_msg("bind error!");
	}
}

void Listen(int backlog) {
	char *ptr;

	if ( (ptr = getenv("LISTENQ")) != nullptr )
		backlog = atoi(ptr);

	if ( listen(serverfd, backlog) == -1 ) {
		error_msg("listen error!");
	}
}

void Start(int protocal, int backlog) {
	Socket(protocal);
	Bind();
	Listen(backlog);

	char buff[1024];
	inet_ntop(AF_INET, &serveraddr.sin_addr, buff, sizeof(buff));
	printf("Server ip is %s running on port %d\n", buff, ntohs(serveraddr.sin_port));
}

void Close() {
	close(serverfd);
	printf("Server closed!\n");
}


int Accept(sockaddr_in &clientaddr) {
	socklen_t clientaddr_len = sizeof(clientaddr);
	int clientfd = accept(serverfd, (struct sockaddr *)&clientaddr, &clientaddr_len);
	return clientfd;
}

void error_msg(string str) {
	printf("%s\n", str.c_str());
	exit(0);
}

void Run() {
	signal(SIGINT, sig_act);
	while(1) {
		sockaddr_in clientaddr;
		int clientfd = Accept(clientaddr);
		int i;
		pthread_mutex_lock(&mv);
		for (i = 0; i < MAX_NUM; ++i)
		{
			if (fd[i] == 0)
			{
				fd[i] = clientfd;
				sock[i] = clientaddr;
				pthread_create(&tid[i], nullptr, Fun, static_cast<void*>(&i));
				break;
			}
		}
		pthread_mutex_unlock(&mv);

		if (i == MAX_NUM) {
			char msg[1024] = "The room is full!";
			write(clientfd, msg, 1024);
			close(clientfd);
		}
	}
}

void* Fun(void *arg) {
	pthread_detach(pthread_self());
	int idx = *static_cast<int*>(arg);

	char buff[1024];
	char msg[1024];

	int len;
	time_t timep;
	inet_ntop(AF_INET, &sock[idx].sin_addr, buff, sizeof(buff));
	printf("connction from %s, port %d\n", buff, ntohs(sock[idx].sin_port));
	first(idx);
	while(1) {
		len = read(fd[idx], buff, 1024);
		time (&timep);
		if (len > 0) {
			printf("receive from %d: %s\n", ntohs(sock[idx].sin_port), buff);
			sprintf(msg, "%sreceive from %s: %s\n", ctime(&timep), clientname[idx].c_str(), buff);
			pthread_mutex_lock(&mv);
			for (int i = 0; i < MAX_NUM; ++i)
			{
				if ((fd[i] != 0) && (i != idx))
				{
					write(fd[i], msg, 1024);
				}
			}
			pthread_mutex_unlock(&mv);
		}
		else {
			pthread_mutex_lock(&mv);
			close(fd[idx]);
			fd[idx] = 0;
			clientname[idx] = "";
			memset(&sock[idx], 0, sizeof(sock[idx]));
			pthread_mutex_unlock(&mv);
			break;
		}
		usleep(1000);
	}
	return nullptr;
}

void sig_act(int signo) {
	for (int i = 0; i < MAX_NUM; ++i)
	{
		if (tid[i] != 0) {
			pthread_join(tid[i], nullptr);
		}
	}
	Close();
	mdb.disconnect();
	exit(0);
}

void first(int idx) {
	char msg[1024] = "Welcome! Choose to sign in or sign up:\n1: sign in\n2: sign up";
	write(fd[idx], msg, 1024);
	char buff[1024];
	read(fd[idx], buff, 1024);
	if (strcmp(buff, "2") == 0) {
		sprintf(msg, "please input your name:");
		write(fd[idx], msg, 1024);
		read(fd[idx], buff, 1024);
		string name(buff);

		sprintf(msg, "please input your password:");
		write(fd[idx], msg, 1024);
		read(fd[idx], buff, 1024);
		string password(buff);

		mdb.insertitem(tablename, " \'" + name + "\', \'" + password + "\'");
	}
	sprintf(msg, "please input your name:");
	write(fd[idx], msg, 1024);
	read(fd[idx], buff, 1024);
	string name(buff);
	string req = "name = \'" + name + "\'";
	string passwd = mdb.selectitem(tablename, "password", req).front().front();
	sprintf(msg, "please input your password:");
	write(fd[idx], msg, 1024);
	read(fd[idx], buff, 1024);
	if (strcmp(buff, passwd.c_str()) == 0) {
		clientname[idx] = name;
		sprintf(msg, "Welcome:%s", name.c_str());
		write(fd[idx], msg, 1024);
	}

}
