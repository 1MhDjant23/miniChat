#pragma once

#include "Libraries.hpp"

class Server {
    private:
        std::vector<int> _sockets;
public:
    void    bindingSock() const;
    Server(); // set up the def sockets for listening.
    ~Server();
};