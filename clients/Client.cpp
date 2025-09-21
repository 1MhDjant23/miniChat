#include "../includes/Client.hpp"
#include <netinet/in.h>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h> 
Client::Client() {}

void    Client::initClient() {
    sockFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockFd <= -1){
        std::cerr << "Socket creation failled: " << strerror(errno) << std::endl;
        exit(-1);
    }
}

void    Client::connectToServer() {
    struct sockaddr_in address;

    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockFd, (struct sockaddr*)&address, sizeof(address)) == -1) 
    {
        std::cerr << strerror(errno) << std::endl;
        close(sockFd);
        exit(-1);
    }
    std::cout << "connected successfully!" << std::endl;
}

void    Client::communication() {
    while(true)
    {
        std::string message;
        std::cout << "message to send:\n->";
        std::getline(std::cin, message);
        if(message == "quit"){
            std::cout << "communication terminated!" << std::endl;
            close(sockFd);
            break;
        }
        const char *mess = message.c_str();
        size_t  length = message.length();

        ssize_t byte_sent = send(sockFd, mess, length, 0);
        if (byte_sent <= -1)
        {
            if(errno == EAGAIN || errno == EWOULDBLOCK)
                std::cout << "maybe buffer is fully." << std::endl;
        }
        std::cout << "waiting for respond ..." << std::endl;
        char    buffer[1024];
        size_t rByte = recv(sockFd, &buffer, sizeof(buffer), 0);
        if(rByte > 0) {
            buffer[rByte] = '\0';
            std::cout << "\033[1;33m" << buffer << "\033[0m" << std::endl;
        }
        if(rByte == 0) 
        {
            std::cout << "connection out-of-line" << std::endl;
            break;
        }
    }
}

Client::~Client() {}