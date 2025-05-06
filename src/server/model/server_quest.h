#pragma once

#include <string>
#include <vector>

#include <model/quest.h>
#include <storage/persistable.h>

namespace Sidequest
{
	namespace Server
	{
		class ServerUser;

		class ServerQuest : public Sidequest::Quest, public Persistable {
		public:
			typedef unsigned long Id;

			ServerQuest(Database* database, Id id);
			ServerQuest(Database* database, Id id, Status status, std::string title, std::string description, ServerUser* owner, ServerUser* editor, Quest* parent );
			~ServerQuest();

			virtual void create_on_database() override;
			virtual void read_on_database() override;
			virtual void update_on_database() override;
			virtual void delete_on_database() override;

			virtual std::string class_id() override;

			Id owner_id;
			Id editor_id;
			Id parent_id;
		};

	}
}
