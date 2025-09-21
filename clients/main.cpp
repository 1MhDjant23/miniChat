#include "../includes/Client.hpp"

int main() {
    Client  user;

    user.initClient();
    user.connectToServer();
    user.communication();
}