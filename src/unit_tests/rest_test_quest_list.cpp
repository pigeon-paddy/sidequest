#include <gtest/gtest.h>

#include <iostream>

#include "server_application.h"
#include "storage/database.h"
#include "client_application.h"
#include "network/client_stubs.h"
#include "network/serialisable_quest.h"

using namespace Sidequest;

#include "rest_test.h"

namespace Sidequst {

	TEST_F(RestTest, QUEST_FETCH_BY_PARENT_1)
	{
		auto parent_quest = new SerialisableQuest(Quest::initial, "Parent Quest", "", nullptr, nullptr, nullptr);
		auto parent_quest_id = client->stubs()->create_quest(parent_quest);

		auto sub_quest = new SerialisableQuest(Quest::initial, "Sub Quest", "", nullptr, nullptr, parent_quest);
		auto sub_quest_id = client->stubs()->create_quest(sub_quest);

		auto list = client->stubs()->quest_by_parent(parent_quest_id);

		client->stubs()->delete_quest(parent_quest_id);
		client->stubs()->delete_quest(sub_quest_id);
	}

	TEST_F(RestTest, QUEST_FETCH_BY_PARENT_5)
	{
		auto parent_quest = new SerialisableQuest(Quest::initial, "Parent Quest", "", nullptr, nullptr, nullptr);
		auto parent_quest_id = client->stubs()->create_quest(parent_quest);

		for (int i = 0; i < 5; i++)
			auto sub_quest = create_subquest(parent_quest);

		auto list = client->stubs()->quest_by_parent(parent_quest_id);

		client->stubs()->delete_quest(parent_quest_id);
	}


}