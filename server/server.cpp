#include "../includes/Server.hpp"
#include <netinet/in.h>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

void    Server::init() {
    int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd <= -1)
    {
        std::cerr << "Socket creation failled: " << strerror(errno) << std::endl;
        exit(-1);
    }
    std::cout << "socket fd is: " << fd << std::endl;
    _socketsFd.push_back(fd);
}

void    Server::bindSock() const {
    struct sockaddr_in address;

    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;
    
    std::cout << "address is: " <<INADDR_ANY << std::endl;

    if (bind(_socketsFd[0], (struct sockaddr*)&address, sizeof(address)) == -1)
    {
        strerror(errno);
        close(_socketsFd[0]);
        exit(1);
    }
    std::cerr << "binding success!" << std::endl;
}

void    Server::listenOnPort() 
{
    if (listen(_socketsFd[0], 5) == -1)
    {
        std::cerr << "listen: " << strerror(errno) << std::endl;
        close(_socketsFd[0]);
        exit(1);
    }
    std::cout << "server listen on 8080" << std::endl;
}

void    Server::acceptConnection()
{
    int clientFd;
    char buffer[1024];
    int byte;

    while (true)
    {
        std::cout << "here we go" << std::endl;
        clientFd = accept(_socketsFd[0], NULL, NULL);
        if (clientFd <= -1)
        {
            close(_socketsFd[0]);
            std::cerr << "accept: " << strerror(errno) << std::endl;
            exit(1);
        }
        std::cout << "a client detected" << std::endl;
        byte = recv(clientFd, buffer, sizeof(buffer), 0);
        buffer[byte] = '\0';
        std::cout << "buffer is: " << buffer << std::endl;
    }
    
}