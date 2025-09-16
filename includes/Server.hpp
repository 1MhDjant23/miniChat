#pragma once

#include "Libraries.hpp"

class Server {
    private:
        std::vector<int> _socketsFd;
    public:
        Server(){};
        void    init();
        void    bindSock() const;
        void    listenOnPort();
        void    acceptConnection();
        ~Server(){};
};