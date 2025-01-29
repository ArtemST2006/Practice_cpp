~Ф#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <thread>
#include <mutex>
#include <cstring>

#define MAXHOSTNAME 256

using namespace std;

extern mutex consoleMutex;

void clearCurrentLine();
void receiveMessages(int socketHandle);
int unix_client();