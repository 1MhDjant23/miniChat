#pragma once

#include "Libraries.hpp"
#include <poll.h>
#define MAX_CONNECTION 100

class Server {
    private:
    std::vector<int> _socketsFd;
    public:
    void    removePollfd(struct pollfd pollFd[], nfds_t &nfds, size_t indexToRemove);
    Server(){};
    void    init();
    void    bindSock() const;
    void    listenOnPort();
    void    acceptConnection();
    void    pollingForConnection();
    void    closeSockets(size_t nfds) const;
    int  nonBlockingMode(int fd);
    ~Server(){};
};