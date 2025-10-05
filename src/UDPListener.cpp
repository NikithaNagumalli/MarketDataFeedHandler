#include "UDPListener.h"

int UDPListener::connect() {
    int status = 0;

    int reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if (bind(sockfd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        status = -1;
        throw std::runtime_error("bind failed");
    }

    ip_mreq group = {};
    group.imr_multiaddr.s_addr = inet_addr(multicast_group.c_str());
    group.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&group, sizeof(group)) < 0) {
        status = -1;
        throw std::runtime_error("setsockopt failed");
    }
    return status;
}
    

int UDPListener::receive(char* buffer) {
    int n = recv(sockfd, buffer, sizeof(Message), 0);
    if (n < 0) {
        cout << "recv failed" << endl;
    }
    if (n == 0) {
        cout << "Socket closed" << endl;
    }
    if (n == sizeof(Message)) {
        Message md;
        memcpy(&md, buffer, sizeof(Message));
        cout << "Parsed message: ts=" << md.timestamp
                << " id=" << md.instrumentId
                << " price=" << md.price
                << " qty=" << md.qty << endl;
        // orderBook.add(md);
    } else {
        cout << "Unexpected size: " << n << " bytes" << endl;
    }
    return n;
}