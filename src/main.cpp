#include <iostream>
#include <sys/socket.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include "../core/Message.h"

using std::cout;
using std::endl;

int main() {
    const std::string multicast_group = "239.255.0.1";
    const int port = 5000;

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    int reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if (bind(sockfd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        throw std::runtime_error("bind failed");
    }

    ip_mreq group = {};
    group.imr_multiaddr.s_addr = inet_addr(multicast_group.c_str());
    group.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&group, sizeof(group)) < 0) {
        throw std::runtime_error("setsockopt failed");
    }


    char buffer[sizeof(Message)];
    cout << "Listening for multicast messages..." << endl;
    while (true) {
        int n = recv(sockfd, buffer, sizeof(Message), 0);
        if (n < 0) {
            cout << "recv failed" << endl;
            continue;
        }
        if (n == 0) {
            cout << "Socket closed" << endl;
            break;
        }
        if (n == sizeof(Message)) {
            Message md;
            memcpy(&md, buffer, sizeof(Message));
            cout << "Parsed message: ts=" << md.timestamp
                 << " id=" << md.instrumentId
                 << " price=" << md.price
                 << " qty=" << md.qty << endl;
        } else {
            cout << "Unexpected size: " << n << " bytes" << endl;
        }
    }
    
    return 0;
}