#include <iostream>
#include <sys/socket.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include "../core/Message.h"

using std::cout;
using std::endl;


static const std::string multicast_group = "239.255.0.1";
static const int port = 5000;

class UDPListener {
    public:
        int connect();
        int receive(char*);

    private:
        const int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
};