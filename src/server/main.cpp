#include <iostream>

#include <network/connection_handler.h>
#include <controller/get_user_command.h>

int main() {
    std::cout << "Sidequest Server " << std::endl;

    auto connection_handler = new Sidequest::Server::ConnectionHandler("localhost", 8000);
    connection_handler->register_command( new Sidequest::Server::GetUserCommand() );
    connection_handler->listen();

    return 0;
}
