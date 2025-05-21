#include "model/quest.h"
#include "model/server_user.h"
#include "storage/database.h"
#include <gtest/gtest.h>
#include <storage/Query.h>



class CRUDTests : public ::testing::Test 
{
protected:
	Sidequest::Server::Database* database;

	CRUDTests() 
	{
	}

	virtual ~CRUDTests() {
	}

	virtual void SetUp() {
		database = new Sidequest::Server::Database("sidequest.db");

		Sidequest::Server::Query query(database,
			"CREATE TABLE IF NOT EXISTS user (email TEXT PRIMARY KEY, display_name TEXT, password TEXT);"
		);
		query.step();
	}

	virtual void TearDown() {
		database->execute("DROP TABLE user;");
		delete database;
	}
};

using namespace Sidequest::Server;

TEST_F(CRUDTests, OPEN_DATABASE)
{
}

TEST_F(CRUDTests, CRUD_RESET_TABLE)
{
	auto user = new ServerUser(database);
	user->reset_user_table();
	delete(user);
}

TEST_F(CRUDTests, CRUD_USER_CREATE_WITH_QUERY)
{
	using namespace Sidequest::Server;

	// 1. User anlegen (INSERT)
	{
		Query insertQuery(database, "INSERT INTO user (email, display_name, password) VALUES (?, ?, ?);");
		insertQuery.bind(1, "crud_user_create@hs-aalen.de");
		insertQuery.bind(2, "Temporary User");
		insertQuery.bind(3, "");
		bool success = insertQuery.step();
		EXPECT_TRUE(success || sqlite3_errcode(database->get_handle()) == SQLITE_DONE);
		// step() returns SQLITE_ROW for SELECT, SQLITE_DONE for INSERT/UPDATE
	}

	// 2. User auslesen (SELECT)
	{
		Query selectQuery(database, "SELECT display_name FROM user WHERE email = ?;");
		selectQuery.bind(1, "crud_user_create@hs-aalen.de");

		bool hasRow = selectQuery.step();
		ASSERT_TRUE(hasRow);  // User muss existieren

		std::string displayName = selectQuery.get_text(0);
		EXPECT_EQ(displayName, "Temporary User");
	}
}

TEST_F(CRUDTests, CRUD_USER_CREATE_DOUBLE)
{
	auto user = new ServerUser(database, "crud_user_create_double@hs-aalen.de", "Temporary User", "");
	user->create_on_database();
	delete(user);

	try {
		auto user = new ServerUser(database, "crud_user_create_double@hs-aalen.de", "Temporary User 2", "");
		user->create_on_database();
		FAIL();
	}
	catch (const UnableToCreateObjectException& expected)
	{
		delete(user);
	}
}

TEST_F(CRUDTests, CRUD_USER_READ)
{
	auto user = new ServerUser(database, "crud_user_read@hs-aalen.de", "Temporary User", "");
	user->create_on_database();
	delete(user);

	user = new ServerUser(database, "crud_user_read@hs-aalen.de");
	user->read_on_database();

	EXPECT_EQ(user->display_name, "Temporary User");
}

TEST_F(CRUDTests, CRUD_USER_UPDATE)
{
	auto user = new ServerUser(database, "crud_user_update@hs-aalen.de", "Temporary User", "");
	user->create_on_database();
	user->display_name = "Changed Display Name";
	user->update_on_database();
	delete(user);

	auto user2 = new ServerUser(database, "crud_user_update@hs-aalen.de");
	user2->read_on_database();

	EXPECT_EQ(user->display_name, "Changed Display Name");
	delete(user2);
}

TEST_F(CRUDTests, CRUD_USER_DELETE)
{
	auto user = new ServerUser(database, "crud_user_delete@hs-aalen.de", "Temporary User", "");
	user->create_on_database();
	delete(user);

	auto user2 = new ServerUser(database, "crud_user_delete@hs-aalen.de");
	user2->delete_on_database();
	delete(user2);

	try {
		auto user3 = new ServerUser(database, "crud_user_delete@hs-aalen.de");
		user3->read_on_database();
		FAIL();
	}
	catch (const UnableToReadObjectException& expected)
	{
		delete(user);
	}
}

TEST_F(CRUDTests, QUEST_CREATE)
{
	auto quest = new Sidequest::Quest("My First Quest", nullptr);

	EXPECT_EQ(quest->getCaption(), "My First Quest");
	EXPECT_EQ(quest->getStatus(), Sidequest::Status::OPEN);
	EXPECT_EQ(quest->findSubquest(9999), nullptr); 

	delete quest;
}

TEST_F(CRUDTests, QUEST_ADD_SUBQUEST)
{
	auto* parent = new Sidequest::Quest("Parent", nullptr);
	auto* sub = new Sidequest::Quest("Child", parent);

	parent->addSubquest(sub);

	auto found = parent->findSubquest(sub->getId());
	ASSERT_NE(found, nullptr);
	EXPECT_EQ(found->getCaption(), "Child");
	EXPECT_EQ(found->getStatus(), Sidequest::Status::OPEN);

	delete parent;  // löscht rekursiv auch sub
}

TEST_F(CRUDTests, QUEST_REMOVE_SUBQUEST)
{
	auto* parent = new Sidequest::Quest("Parent", nullptr);
	auto* sub = new Sidequest::Quest("Child", parent);

	parent->addSubquest(sub);
	EXPECT_NE(parent->findSubquest(sub->getId()), nullptr);

	parent->removeSubquest(sub->getId());
	EXPECT_EQ(parent->findSubquest(sub->getId()), nullptr);

	delete parent;  // sub wurde bereits gelöscht
}

TEST_F(CRUDTests, QUEST_UPDATE_STATUS)
{
	auto* quest = new Sidequest::Quest("Status Test", nullptr);
	quest->updateStatus(Sidequest::Status::FINALIZED);

	EXPECT_EQ(quest->getStatus(), Sidequest::Status::FINALIZED);

	delete quest;
}

TEST_F(CRUDTests, QUEST_MULTIPLE_SUBQUESTS)
{
	auto* main = new Sidequest::Quest("Main", nullptr);
	auto* sub1 = new Sidequest::Quest("Sub 1", main);
	auto* sub2 = new Sidequest::Quest("Sub 2", main);

	main->addSubquest(sub1);
	main->addSubquest(sub2);

	EXPECT_EQ(main->findSubquest(sub1->getId())->getCaption(), "Sub 1");
	EXPECT_EQ(main->findSubquest(sub2->getId())->getCaption(), "Sub 2");

	main->removeSubquest(sub1->getId());
	EXPECT_EQ(main->findSubquest(sub1->getId()), nullptr);
	EXPECT_NE(main->findSubquest(sub2->getId()), nullptr);

	delete main;
}
