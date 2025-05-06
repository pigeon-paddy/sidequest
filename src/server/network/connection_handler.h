#pragma once

#include <string>
#include <unordered_map>

namespace httplib
{
	class Server;
}

namespace Sidequest
{
	namespace Server
	{
		class CommandHandler;

		class ConnectionHandler
		{
		public:
			ConnectionHandler( std::string hostname, int port );
			~ConnectionHandler();
			
			void register_command( CommandHandler* handler );

			void listen();

		protected:
			std::unordered_map< std::string, CommandHandler* > endpoints;
			httplib::Server* server;
			std::string hostname;
			int port;
		};

	}
}