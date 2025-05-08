#pragma once

#include <string>

#include "network/command_handler.h"

namespace Sidequest
{
	namespace Server
	{
		class Database;

		class UserUpdateCommand: public CommandHandler
		{
		public:
			UserUpdateCommand(Database* database);
			virtual void execute(const httplib::Request& request, httplib::Response& response) override;
			virtual std::string endpoint() override;
		protected:
			Database* database;
		};

	}
}