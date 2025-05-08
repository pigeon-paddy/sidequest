#pragma once

#include "connection_handler.h"

#include "httplib.h"

#include "network/command_handler.h"

namespace Sidequest
{
    namespace Server
    {
        ConnectionHandler::ConnectionHandler(std::string hostname, int port)
            : hostname( hostname ), port( port )
        {
            server = new httplib::Server();
        }

        ConnectionHandler::~ConnectionHandler()
        {
            for (auto& it : endpoints)
                delete( it.second );
        }
         
        void ConnectionHandler::register_get_command(CommandHandler* command)
        {
            auto function = command->get_function();
            server->Get(command->endpoint(), function);
        }

        void ConnectionHandler::register_put_command(CommandHandler* command)
        {
            auto function = command->get_function();
            server->Put(command->endpoint(), function);
        }

        void ConnectionHandler::listen()
        {
            server->listen(hostname, port);
        }
    } // namespace Server
} // namespace Sidequest