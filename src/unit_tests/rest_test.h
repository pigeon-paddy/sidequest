#include <gtest/gtest.h>

#include <iostream>

#include "network/serialisable_user.h"

using namespace Sidequest;

namespace Sidequest 
{
	namespace Server {
		class Database;
		class ServerApplication;
	}
	namespace Client {
		class ClientApplication;
	}

	class RestTest : public ::testing::Test
	{
	protected:

		RestTest();

		virtual ~RestTest();

		virtual void SetUp();

		virtual void TearDown();

		Server::Database* database;
		Server::ServerApplication* server;
		std::thread thread;
		Client::ClientApplication* client;
	};

}