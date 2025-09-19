#pragma once

#include "Libraries.hpp"

#define MAX_CONNECTION 100

class Server {
    private:
        std::vector<int> _socketsFd;
    public:
        Server(){};
        void    init();
        void    bindSock() const;
        void    listenOnPort();
        void    acceptConnection();
        void    pollingForConnection();
        ~Server(){};
};