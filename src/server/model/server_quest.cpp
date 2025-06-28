#include "server_quest.h"

#include "storage/database.h"
#include "server_user.h"

namespace Sidequest 
{
	namespace Server 
	{
	
		ServerQuest::ServerQuest(Database* database, Id id)
			: Persistable(database)
			, Quest(id)
		{
		}

		ServerQuest::ServerQuest(Database* database, Status status, std::string title, std::string description, ServerUser* owner, ServerUser* editor, Quest* parent)
			: Persistable(database)
			, Quest( status, title, description, owner, editor, parent )
		{
		}

		ServerQuest::~ServerQuest()
		{
		}

		void ServerQuest::create_on_database()
		{
			auto query = Query(database, "INSERT INTO quest(title, description, status, owner, editor, parent ) VALUES (?, ?, ?, ?, ?, ?);");
			bind_all_parameters(query);

			query.next_row();
			if (!query.is_done())
				throw UnableToCreateObjectException(title);

			id = query.last_insert_rowid();
		}

		void ServerQuest::read_on_database()
		{
			auto query = Query(database, "SELECT id, title, description, status, owner, editor, parent FROM quest WHERE id=?;");
			query.bind(1, id);
			query.next_row();
			if (!query.has_rows())
				throw UnableToReadObjectException("quest: " + std::to_string(id) );
			id          = query.int_value("id");
			title       = query.text_value("title");
			description = query.text_value("description");
			std::string status_string = query.text_value("status");
			status      = Quest::string_to_status(status_string);
			owner_id    = query.optional_int_value("owner");
			editor_id   = query.optional_int_value("editor");
			parent_id   = query.optional_int_value("parent");
		}

		void ServerQuest::update_on_database()
		{
			auto query = Query(database, "UPDATE quest set title=?, description=?, status=?, owner=?, editor=?, parent=? WHERE id=?;");
			bind_all_parameters(query);
			query.bind(7, id);

			query.next_row();
			if (!query.is_done())
				throw UnableToCreateObjectException(title);

			id = query.last_insert_rowid();
		}

		void ServerQuest::delete_on_database()
		{
			auto query = Query(database, "DELETE FROM quest WHERE id=?;");
			query.bind(1, id);
			query.next_row();
			if (!query.is_done())
				throw UnableToDeleteObjectException( "quest: " + std::to_string(id) );
		}

		std::string ServerQuest::class_id()
		{
			return "quest";
		}
		
		// Used for these two queries:
		// "UPDATE quest set title=?, description=?, status=?, owner=?, editor=?, parent=? WHERE id=?;"
		// "INSERT INTO quest(title, description, status, owner, editor, parent ) VALUES (?, ?, ?, ?, ?, ?);"
		void ServerQuest::bind_all_parameters(Query& query)
		{
			query.bind(1, title);
			query.bind(2, description);
			query.bind(3, status_to_string(status));
			if (owner != nullptr)
				query.bind(4, owner->email);
			else
				query.bind_null(4);

			if (editor != nullptr)
				query.bind(5, editor->email);
			else
				query.bind_null(5);

			if (parent != nullptr)
				query.bind(6, parent->id);
			else
				query.bind_null(6);

		}

	}
}