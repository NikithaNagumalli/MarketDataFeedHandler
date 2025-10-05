#include "UDPListener.h"

int main() {

    UDPListener udpListener;
    if (udpListener.connect() == -1) {
        cout << "unable to connect to multicast source" << endl;
    }

    char buffer[sizeof(Message)];
    cout << "Listening for multicast messages..." << endl;
    while (true) {
        udpListener.receive(buffer); //TODO: handle errors in receive
    }
    
    return 0;
}