#include "../includes/Libraries.hpp"
#include "Server.hpp"

Server::Server() {
    int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd <= -1)
    {
        std::cerr << "Socket creation failled: " << strerror(errno) << std::endl;
        exit(-1);
    }
    _sockets.push_back(fd);
}

void    Server::bindingSock() const {

}