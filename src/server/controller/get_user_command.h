#pragma once

#include <string>

#include "network/command_handler.h"

namespace Sidequest
{
	namespace Server
	{
		class GetUserCommand: public CommandHandler
		{
		public:
			virtual void execute(const httplib::Request& request, httplib::Response& response) override;
			virtual std::string endpoint() override;
		};

	}
}