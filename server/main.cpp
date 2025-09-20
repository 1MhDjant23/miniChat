#include "../includes/Server.hpp"

int main() 
{
    Server  server;
    server.init(); //creating sockets;
    server.bindSock();
    server.listenOnPort();
    // server.acceptConnection();
    server.pollingForConnection();
    // while (true)
    // {

    // }



    return 0;
}