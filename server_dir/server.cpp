#include "server.h"


int main() {
    Server server(8080,"storage.db");
    server.communication();
    return 0;
}