#include <iostream>
#include <sys/socket.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include "OrderBook.h"

using std::cout;
using std::endl;


static const std::string multicast_group = "239.255.0.1";
static const int port = 5000;

class UDPListener {
    public:
        UDPListener() { sockfd = socket(AF_INET, SOCK_DGRAM, 0);}
        ~UDPListener() {
            if (sockfd > 0) {
                close(sockfd);
            }
        }
        UDPListener(const UDPListener&) = delete;
        UDPListener& operator=(const UDPListener&) = delete;
        UDPListener(UDPListener&&) = delete;
        UDPListener& operator=(UDPListener&&) = delete;
        int connect();
        int receive(char*);

    private:
        int sockfd;
        OrderBook orderBook;
};