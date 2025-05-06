#pragma once

#include <string>

#include "httplib.h"

namespace Sidequest
{
	namespace Server
	{
		class CommandHandler
		{
		public:
			virtual void execute(const httplib::Request& request, httplib::Response& response) = 0;
			virtual std::string endpoint() = 0;

			std::function<void(const httplib::Request&, httplib::Response&)> get_function();
		};

	}
}