#include "model/quest.h"
#include "model/server_user.h"
#include "storage/database.h"
#include <gtest/gtest.h>
#include "storage/Query.h"

#include "storage/database.h"
#include "model/server_user.h"


class CRUDTestsUser : public ::testing::Test 
{
protected:
	Sidequest::Server::Database* database;

	CRUDTestsUser()
	{
	}

	virtual ~CRUDTestsUser() {
	}

	virtual void SetUp() {
<<<<<<<< HEAD:src/unit_tests/crud_test.cpp
		database = new Sidequest::Server::Database("sidequest.db");

		Sidequest::Server::Query query(database,
			"CREATE TABLE IF NOT EXISTS user (email TEXT PRIMARY KEY, display_name TEXT, password TEXT);"
		);
		query.step();
========
		database = new Sidequest::Server::Database(":memory:");
		database->execute_sql_statement("CREATE TABLE user(id INTEGER PRIMARY KEY AUTOINCREMENT, email TEXT unique, display_name TEXT, password TEXT);");
>>>>>>>> origin/fetch_quest:src/unit_tests/crud_test_user.cpp
	}

	virtual void TearDown() {
		database->execute("DROP TABLE user;");
		delete database;
	}
};

using namespace Sidequest::Server;

TEST_F(CRUDTestsUser, OPEN_DATABASE)
{
}

<<<<<<<< HEAD:src/unit_tests/crud_test.cpp
TEST_F(CRUDTests, CRUD_RESET_TABLE)
{
	auto user = new ServerUser(database);
	user->reset_user_table();
========
TEST_F(CRUDTestsUser, CRUD_USER_CREATE)
{
	auto user = new ServerUser( database, "crud_user_create@hs-aalen.de", "Temporary User", "");
	user->create_on_database();
	auto id = user->id;
>>>>>>>> origin/fetch_quest:src/unit_tests/crud_test_user.cpp
	delete(user);
}

<<<<<<<< HEAD:src/unit_tests/crud_test.cpp
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
========
	auto user2 = new ServerUser(database);
	user2->read_on_database(id);

	EXPECT_EQ(user2->display_name, "Temporary User");
	delete(user2);
>>>>>>>> origin/fetch_quest:src/unit_tests/crud_test_user.cpp
}

TEST_F(CRUDTestsUser, CRUD_USER_CREATE_DOUBLE)
{
	auto user = new ServerUser(database, "crud_user_create_double@hs-aalen.de", "Temporary User", "");
	user->create_on_database();
	delete(user);

	try {
		user = new ServerUser(database, "crud_user_create_double@hs-aalen.de", "Temporary User 2", "");
		user->create_on_database();
		FAIL();
	}
	catch (const UnableToCreateObjectException& expected) {
		delete(user);
	}
}

TEST_F(CRUDTestsUser, CRUD_USER_READ)
{
	auto user = new ServerUser(database, "crud_user_read@hs-aalen.de", "Temporary User", "");
	user->create_on_database();
	auto id = user->id;
	delete(user);

	user = new ServerUser(database);
	user->read_on_database(id);

	EXPECT_EQ(user->display_name, "Temporary User");
}

TEST_F(CRUDTestsUser, CRUD_USER_UPDATE)
{
	auto user = new ServerUser(database, "crud_user_update@hs-aalen.de", "Temporary User", "");
	user->create_on_database();
	auto id = user->id;
	user->display_name = "Changed Display Name";
	user->update_on_database();
	delete(user);

	auto user2 = new ServerUser(database);
	user2->read_on_database(id);

	EXPECT_EQ(user2->display_name, "Changed Display Name");
	delete(user2);
}

TEST_F(CRUDTestsUser, CRUD_USER_DELETE)
{
	auto user = new ServerUser(database, "crud_user_delete@hs-aalen.de", "Temporary User", "");
	user->create_on_database();
	auto id = user->id;
	delete(user);

	auto user2 = new ServerUser(database, id);
	user2->delete_on_database();
	delete(user2);

	auto user3 = new ServerUser(database);
	try {
		user3->read_on_database(id);
		FAIL();
	}
	catch (const UnableToReadObjectException& expected)
	{
		delete(user3);
	}
}

TEST_F(CRUDTestsUser, CRUD_USER_DELETE_NON_EXISTENT)
{
	auto user = new ServerUser(database, 15);

	try {
		user->delete_on_database();
		FAIL();
	}
	catch (const UnableToDeleteObjectException& expected) {
		delete(user);
	}
}

TEST_F(CRUDTests, QUEST_CREATE)
{
	auto quest = new Sidequest::Quest("My First Quest", nullptr);

	EXPECT_EQ(quest->getCaption(), "My First Quest");
	EXPECT_EQ(quest->getStatus(), Sidequest::Status::initial);
	EXPECT_EQ(quest->findSubquest(9999), nullptr); 

	delete (quest);
}

TEST_F(CRUDTests, QUEST_ADD_SUBQUEST)
{
	auto* parent = new Sidequest::Quest("Parent", nullptr);
	auto* sub = new Sidequest::Quest("Child", parent);

	parent->addSubquest(sub);

	auto found = parent->findSubquest(sub->getId());
	ASSERT_NE(found, nullptr);
	EXPECT_EQ(found->getCaption(), "Child");
	EXPECT_EQ(found->getStatus(), Sidequest::Status::initial);

	delete parent;  // l�scht rekursiv auch sub
}

TEST_F(CRUDTests, QUEST_REMOVE_SUBQUEST)
{
	auto* parent = new Sidequest::Quest("Parent", nullptr);
	auto* sub = new Sidequest::Quest("Child", parent);

	parent->addSubquest(sub);
	EXPECT_NE(parent->findSubquest(sub->getId()), nullptr);

	parent->removeSubquest(sub->getId());
	EXPECT_EQ(parent->findSubquest(sub->getId()), nullptr);

	delete parent;  // sub wurde bereits gel�scht
}

TEST_F(CRUDTests, QUEST_UPDATE_STATUS)
{
	auto* quest = new Sidequest::Quest("Status Test", nullptr);
	quest->updateStatus(Sidequest::Status::inactive);

	EXPECT_EQ(quest->getStatus(), Sidequest::Status::inactive);

	delete (quest);
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
