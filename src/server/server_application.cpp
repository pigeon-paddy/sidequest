#include <server_application.h>

#include <iostream>
#include <chrono>
#include <thread>

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

namespace Sidequest
{
	namespace Server
	{

		ServerApplication::ServerApplication(std::string hostname, int port)
		{
			connection_handler = new ConnectionHandler( hostname, port );
			database = new Sidequest::Server::Database("sidequest.db");
			owns_database = true;
			setup_commands();
		}

		ServerApplication::ServerApplication(std::string hostname, int port, Database* database)
			: database(database)
		{
			connection_handler = new ConnectionHandler(hostname, port);
			owns_database = false;
			setup_commands();
		}

		ServerApplication::~ServerApplication()
		{
			if (owns_database)
				delete(database);
			delete(connection_handler);
		}

		void ServerApplication::run()
		{
			connection_handler->listen();
		}

		void ServerApplication::stop()
		{
			connection_handler->stop();
		}

		void ServerApplication::setup_commands()
		{
			connection_handler->register_get_command(new Sidequest::Server::UserReadCommand(database));
			connection_handler->register_put_command(new Sidequest::Server::UserCreateCommand(database));
			connection_handler->register_put_command(new Sidequest::Server::UserUpdateCommand(database));
			connection_handler->register_delete_command(new Sidequest::Server::UserDeleteCommand(database));

			connection_handler->register_get_command(new Sidequest::Server::QuestReadCommand(database));
			connection_handler->register_put_command(new Sidequest::Server::QuestCreateCommand(database));
			connection_handler->register_put_command(new Sidequest::Server::QuestUpdateCommand(database));
			connection_handler->register_delete_command(new Sidequest::Server::QuestDeleteCommand(database));
		}

	}
}