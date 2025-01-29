#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#include <stdlib.h>
#include <thread>
#include <strings.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <vector>
#include <netinet/in.h>
#include <cstring>

#define MAXHOSTNAME 256

using namespace std;

extern int coutOfConnections;
extern vector<int> clients;

void handleClient(int);
int unix_server();