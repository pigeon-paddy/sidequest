#include <gtest/gtest.h>

#include "storage/database.h"
#include "model/server_quest.h"

class CRUDTestsQuest : public ::testing::Test 
{
protected:
	Sidequest::Server::Database* database;

	CRUDTestsQuest()
	{
	}

	virtual ~CRUDTestsQuest() {
	}

	virtual void SetUp() {
		database = new Sidequest::Server::Database(":memory:");
		database->execute_sql_statement("CREATE TABLE quest(id INTEGER PRIMARY KEY AUTOINCREMENT, status TEXT, title TEXT, description TEXT, owner INTEGER, editor INTEGER, parent INTEGER);");
	}

	virtual void TearDown() {
		delete database;
	}
};

using namespace Sidequest::Server;

TEST_F(CRUDTestsQuest, CRUD_QUEST_CREATE)
{
	auto quest = new ServerQuest( database, ServerQuest::Status::initial, "Example Todo", "", nullptr, nullptr, nullptr );
	quest->create_on_database();
	auto id = quest->id;
	delete(quest);

	auto quest2 = new ServerQuest(database, id);
	quest2->read_on_database();

	EXPECT_EQ(quest->status, ServerQuest::Status::initial);
	EXPECT_EQ(quest->title, "Example Todo");
	EXPECT_EQ(quest->owner_id, std::nullopt);
	delete(quest2);
}

TEST_F(CRUDTestsQuest, CRUD_QUEST_READ)
{
	auto quest = new ServerQuest(database, ServerQuest::Status::initial, "Example Todo", "", nullptr, nullptr, nullptr);
	quest->create_on_database();
	auto id = quest->id;
	delete(quest);

	quest = new ServerQuest(database, id);
	quest->read_on_database();

	EXPECT_EQ(quest->title, "Example Todo");
	EXPECT_EQ(quest->description, "");
	EXPECT_EQ(quest->status, ServerQuest::Status::initial);
	EXPECT_EQ(quest->owner, nullptr);
	EXPECT_EQ(quest->owner_id, std::nullopt);
	EXPECT_EQ(quest->editor, nullptr);
	EXPECT_EQ(quest->editor_id, std::nullopt);
	EXPECT_EQ(quest->parent, nullptr);
	EXPECT_EQ(quest->parent_id, std::nullopt);
}

TEST_F(CRUDTestsQuest, CRUD_QUEST_UPDATE)
{
	auto quest = new ServerQuest(database, ServerQuest::Status::initial, "Example Todo", "", nullptr, nullptr, nullptr);
	quest->create_on_database();
	auto id = quest->id;
	quest->title = "Changed Quest Title";
	quest->status = ServerQuest::Status::done;
	quest->update_on_database();
	delete(quest);

	auto quest2 = new ServerQuest(database, id);
	quest2->read_on_database();

	EXPECT_EQ(quest2->title, "Changed Quest Title");
	EXPECT_EQ(quest2->status, ServerQuest::Status::done);
	delete(quest2);
}

TEST_F(CRUDTestsQuest, CRUD_QUEST_DELETE)
{
	auto quest = new ServerQuest(database, ServerQuest::Status::initial, "Example Todo", "", nullptr, nullptr, nullptr);
	quest->create_on_database();
	auto id = quest->id;
	delete(quest);

	auto quest2 = new ServerQuest(database, id);
	quest2->delete_on_database();
	delete(quest2);

	auto quest3 = new ServerQuest(database, id);
	try {
		quest3->read_on_database();
		FAIL();
	}
	catch (const UnableToReadObjectException& expected)
	{
		delete(quest3);
	}
}
