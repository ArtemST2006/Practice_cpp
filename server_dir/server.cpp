#include "server.h"


int main() {
    Server server(8080);
    server.communication();

    return 0;
}