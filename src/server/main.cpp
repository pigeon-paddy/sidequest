#include <iostream>

#include <storage/database.h>
#include <network/connection_handler.h>

#include <controller/user_crud/user_read_command.h>
#include <controller/user_crud/user_create_command.h>
#include <controller/user_crud/user_update_command.h>
#include <controller/user_crud/user_delete_command.h>

#include <controller/quest_crud/quest_read_command.h>
#include <controller/quest_crud/quest_create_command.h>
#include <controller/quest_crud/quest_update_command.h>
#include <controller/quest_crud/quest_delete_command.h>

int main() {
    std::cout << "Sidequest Server " << std::endl;

    auto database = new Sidequest::Server::Database("sidequest.db");

    auto connection_handler = new Sidequest::Server::ConnectionHandler("localhost", 8000);

    connection_handler->register_get_command(new Sidequest::Server::UserReadCommand(database));
    connection_handler->register_put_command(new Sidequest::Server::UserCreateCommand(database));
    connection_handler->register_put_command(new Sidequest::Server::UserUpdateCommand(database));
    connection_handler->register_put_command(new Sidequest::Server::UserDeleteCommand(database));

    connection_handler->register_get_command(new Sidequest::Server::QuestReadCommand(database));
    connection_handler->register_put_command(new Sidequest::Server::QuestCreateCommand(database));
    connection_handler->register_put_command(new Sidequest::Server::QuestUpdateCommand(database));
    connection_handler->register_put_command(new Sidequest::Server::QuestDeleteCommand(database));

    connection_handler->listen();

    return 0;
}
