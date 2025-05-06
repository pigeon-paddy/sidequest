#pragma once

#include <string>
#include <vector>
#include <sqlite3.h>

#include "prepared_statement.h"

/* Base class of all persistent domain model objects on the server
*  that implement CRUDS operations */
namespace Sidequest
{
	namespace Server
	{
		class Database;

		/* All persistent classes should derive from Persistable */
		class Query
		{
		public:
			Query(Database* database, std::string statement_sql);
			Query(const Query& other);
			virtual ~Query();

			const Query& operator=(const Query& other);

			void bind(int parameter_index, std::string value);
			void bind(int parameter_index, unsigned int value);

			int execute();

			int read_int_value(std::string column_name);
			std::string read_text_value(std::string column_name);

		protected:
			void reset();

		protected:
			Database* database;
			PreparedStatement* prepared_statement;
			int error_code;
		};

	};
}