#include <gtest/gtest.h>

#include <iostream>

#include "network/serialisable_user.h"

using namespace Sidequest;

#include "server_application.h"
#include "storage/database.h"
#include "client_application.h"
#include "network/client_stubs.h"

class RestTestCrudUser : public ::testing::Test
{
protected:

	RestTestCrudUser()
	{
	}

	virtual ~RestTestCrudUser() {
	}

	virtual void SetUp()
	{
		database = new Sidequest::Server::Database(":memory:");
		database->execute_sql_statement("CREATE TABLE user(id INTEGER PRIMARY KEY AUTOINCREMENT, email TEXT unique, display_name TEXT, password TEXT);");

		server = new Sidequest::Server::ServerApplication("localhost", 80, database);
		thread = std::thread( &Sidequest::Server::ServerApplication::run, server);

		client = new Sidequest::Client::ClientApplication("localhost", 80);
	}

	virtual void TearDown()
	{
		delete(database);

		server->stop();
		thread.join();
		delete(server);

		delete(client);
	}

	Sidequest::Server::Database* database;
	Sidequest::Server::ServerApplication* server;
	std::thread thread;
	Sidequest::Client::ClientApplication* client;
};


TEST_F(RestTestCrudUser, CREATE_THEN_DELETE)
{
	auto user = new SerialisableUser("test@hs-aalen.de", "REST Test User", "");
	auto id = client->stubs()->createUser( user );

	client->stubs()->deleteUser(id);
}

TEST_F(RestTestCrudUser, CREATE_THEN_READ)
{
	auto user1 = new SerialisableUser("test@hs-aalen.de", "REST Test User", "");
	auto id = client->stubs()->createUser(user1);

	auto user2 = client->stubs()->readUser(id);

	ASSERT_EQ(id,                  user2->id);
	ASSERT_EQ(user1->display_name, user2->display_name);
	ASSERT_EQ(user1->email,        user2->email);

	delete(user1);
	delete(user2);
}

TEST_F(RestTestCrudUser, CREATE_THEN_UPDATE_THEN_READ)
{
	auto user1 = new SerialisableUser("test@hs-aalen.de", "REST Test User", "");
	auto id = client->stubs()->createUser(user1);

	auto user2 = client->stubs()->readUser(id);

	ASSERT_EQ(id, user2->id);
	ASSERT_EQ(user1->display_name, user2->display_name);
	ASSERT_EQ(user1->email, user2->email);

	user2->display_name = "UPDATED Test User";

	client->stubs()->updateUser(user2);

	auto user3 = client->stubs()->readUser(id);

	ASSERT_EQ(id, user3->id);
	ASSERT_EQ("UPDATED Test User", user3->display_name);
	ASSERT_EQ(user1->email, user3->email);

	delete(user1);
	delete(user2);
	delete(user3);
}
