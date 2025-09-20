#include "../includes/Server.hpp"
#include <netinet/in.h>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>

void    Server::init() {
    int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd <= -1)
    {
        std::cerr << "Socket creation failled: " << strerror(errno) << std::endl;
        exit(-1);
    }
    _socketsFd.push_back(fd);
    std::cout << "setting server non-blocking ..." << std::endl;
    nonBlockingMode(fd); // make this socket as non-blocking I/O. 
}

void    Server::bindSock() const {
    struct sockaddr_in address;

    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;
    
    // std::cout << "address is: " <<INADDR_ANY << std::endl;
    if (bind(_socketsFd[0], (struct sockaddr*)&address, sizeof(address)) == -1)
    {
        strerror(errno);
        close(_socketsFd[0]);
        exit(1);
    }
    std::cout << "bind .............." << std::endl;
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

void    Server::closeSockets(size_t nfds) const {
    for (size_t i = 0; i < nfds; i++)
        close(_socketsFd[i]);
}
int Server::nonBlockingMode(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void    Server::removePollfd(struct pollfd pollFd[], nfds_t &nfds, size_t indexToRemove) {
    if (indexToRemove >= nfds)
        return ;
    for (size_t i = indexToRemove; i < nfds; i++)
        pollFd[i] = pollFd[i + 1];
    nfds--;
}

void    Server::pollingForConnection() {
    struct  pollfd  pollFd[MAX_CONNECTION];
    nfds_t          nfds = 1;
    // int             ret;
    int             nByte;

    pollFd[0].fd = _socketsFd[0];
    pollFd[0].events = POLLIN;

    while (true)
    {
        std::cout << "waitting for events ..." << std::endl;
        std::cout << "total of nfds " << nfds << std::endl;
        int ret = poll(pollFd, nfds, -1);
        std::cout << ret << " events occured." << std::endl;
        if (ret > 0)
        {
            if (pollFd[0].revents == POLLIN) // adding new connection to server
            {
                std::cout << "---------- accepting new connection ... ----------" << std::endl;
                int clientFd = accept(_socketsFd[0], NULL, NULL);
                if (clientFd <= -1)
                {
                    if (errno == EWOULDBLOCK || errno == EAGAIN)
                        std::cout << "no incoming connection." << std::endl;
                }
                else
                {
                    _socketsFd.push_back(clientFd);
                    nonBlockingMode(clientFd);
                    pollFd[nfds].fd = clientFd;
                    pollFd[nfds].events = POLLIN | POLLOUT;
                    nfds++;
                }
            }
            for (size_t i = 1; i < nfds; i++)
            {
                if (pollFd[i].revents & POLLIN)
                {
                    std::cout << "client send data ..." << std::endl;
                    char    buffer[1024];
                    if ((nByte = recv(pollFd[i].fd, buffer, sizeof(buffer), 0)) > 0)
                    {
                        buffer[nByte] = '\0';
                        std::cout <<  buffer << std::endl;
                    }
                    else if (nByte == 0)
                    {
                        std::cout << "connection closed by the peer." << std::endl;
                        close(pollFd[i].fd);
                        _socketsFd.erase(_socketsFd.begin() + i);
                        removePollfd(pollFd, nfds, i); //remove the user i.
                        // nfds--;
                    }
                    else if (errno == EAGAIN || errno == EWOULDBLOCK)
                    {
                        std::cout << "user still connected" << std::endl;
                    }
                }
                if (pollFd[i].revents & POLLOUT)
                {
                    const char *messg = "i'm there! You got it!?\n";
                    std::cout << "<<<<<<<<<<<<< client need data >>>>>>>>>>>>>" << std::endl;
                    ssize_t byte;
                    if ((byte = send(pollFd[i].fd, messg, strlen(messg), 0)) > 0)
                    {
                        std::cout << "sizeof messg: " << strlen(messg) << ", byte sending: " << byte << std::endl;
                        if ((size_t)byte < sizeof(messg))
                        {
                            byte = send(pollFd[i].fd, &messg[byte - 1], sizeof(messg) - byte, 0);
                        }
                        
                        std::cout << "message has been sent successfully" << std::endl;
                        close(pollFd[i].fd);
                        removePollfd(pollFd, nfds, i);
                        _socketsFd.erase(_socketsFd.begin() + i);
                    }
                    else if (byte <= -1) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                        {
                            std::cout << "maybe buffer is fully" << std::endl;
                        }
                    }
                }
                std::cout << "users connected ..." << std::endl;
                for (size_t i = 0; i < nfds; i++)
                    std::cout << "pollfd " << i + 1 << ":--> " << pollFd[i].fd << std::endl;
            }
        }
        else if (ret < 0){
            closeSockets(nfds);
            strerror(errno);
            exit(EXIT_FAILURE);
        }
    }
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
        while ((byte = recv(clientFd, buffer, sizeof(buffer), 0)) > 0)
        {
            buffer[byte] = '\0';
            std::cout << "buffer is: " << buffer << std::endl;
        }
        if (byte == 0)
        {
            std::cout << "connection closed by client: " << clientFd << std::endl;
        }
        if (byte == -1)
        {
            close(clientFd);
            std::cerr << "fail receiving from client: " << strerror(errno) << std::endl;
        }
    }
}