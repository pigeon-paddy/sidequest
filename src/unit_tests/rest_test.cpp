#include <gtest/gtest.h>

#include <iostream>
#include <thread>

#include "server_application.h"
#include "storage/database.h"
#include "client_application.h"
#include "network/client_stubs.h"
#include "network/serialisable_user.h"

#include "rest_test.h"

namespace Sidequest {

	RestTest::RestTest()
	{
	}

	RestTest::~RestTest()
	{
	}

	void RestTest::SetUp()
	{
		database = new Server::Database(":memory:");
		database->execute_sql_statement("CREATE TABLE user(id INTEGER PRIMARY KEY AUTOINCREMENT, email TEXT unique, display_name TEXT, password TEXT);");
		database->execute_sql_statement("CREATE TABLE quest(id INTEGER PRIMARY KEY AUTOINCREMENT, status TEXT, title TEXT, description TEXT, owner INTEGER, editor INTEGER, parent INTEGER);");

		server = new Server::ServerApplication("localhost", 80, database);
		thread = std::thread( &Server::ServerApplication::run, server);

		client = new Client::ClientApplication("localhost", 80);
	}

	void RestTest::TearDown()
	{
		delete(database);

		server->stop();
		thread.join();
		delete(server);

		delete(client);
	}

}