#include "server_user.h"

#include "storage/database.h"

namespace Sidequest 
{
	namespace Server {

		ServerUser::ServerUser(Database* database)
			: Persistable(database)
		{
		}		
		
		ServerUser::ServerUser(Database* database, std::string email)
			: Persistable(database)
			, User(email)
		{
		}

		ServerUser::ServerUser(Database* database, std::string email, std::string display_name, std::string password)
			: Persistable(database)
			, User(email, display_name, password)
		{
		}

		ServerUser::~ServerUser()
		{
		}

		void ServerUser::create_user_table()
		{
			auto prepared_statement = database->prepare("create table user(email text primary key, display_name text, password text);");
			if (database->execute(prepared_statement) != SQLITE_DONE)
				throw UnableToCreateObjectException("User table");
			database->reset_statement(prepared_statement);
		}

		void ServerUser::clear_user_table()
		{
			auto database = new Database("sidequest.db");
			database->execute("DROP TABLE user;");
			delete database;
		}

		void ServerUser::reset_user_table()
		{
			clear_user_table();
			create_user_table();
		}

		void ServerUser::create_on_database()
		{
			auto prepared_statement = database->prepare( "INSERT INTO user(email, display_name, password) VALUES (?, ?, ?);" );
			database->bind(prepared_statement, 1, email);
			database->bind(prepared_statement, 2, display_name);
			database->bind(prepared_statement, 3, password);
			if ( database->execute(prepared_statement) != SQLITE_DONE )
				throw UnableToCreateObjectException(email);
			database->reset_statement(prepared_statement);
		}

		void ServerUser::read_on_database()
		{
			auto prepared_statement = database->prepare("SELECT * FROM user WHERE email = ?;");
			database->bind(prepared_statement, 1, email);
			if ( database->execute(prepared_statement) != SQLITE_ROW )
				throw UnableToReadObjectException(email);
			display_name = database->read_text_value(prepared_statement, "display_name");
			password     = database->read_text_value(prepared_statement, "password");
			database->reset_statement(prepared_statement);
		}

		void ServerUser::update_on_database()
		{
			auto prepared_statement = database->prepare("UPDATE user set display_name=?, password=? WHERE email=?;");
			database->bind(prepared_statement, 1, display_name);
			database->bind(prepared_statement, 2, password);
			database->bind(prepared_statement, 3, email);
			if ( database->execute(prepared_statement) != SQLITE_DONE )
				throw UnableToUpdateObjectException(email);
			database->reset_statement(prepared_statement);
		}

		void ServerUser::delete_on_database()
		{
			auto prepared_statement = database->prepare("DELETE FROM user WHERE email=?;");
			database->bind(prepared_statement, 1, email);
			if ( database->execute(prepared_statement) != SQLITE_DONE )
				throw UnableToDeleteObjectException(email);
			database->reset_statement(prepared_statement);
		}

		std::string ServerUser::class_id()
		{
			return "user";
		}



	}
}