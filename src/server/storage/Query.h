#pragma once
#include <string>
#include <sqlite3.h>

namespace Sidequest::Server {

    class Database;  // forward declaration

    class Query {
    public:
        Query(Database* db, const std::string& sql);
        ~Query();

        void bind(int index, const std::string& value);
        void bind(int index, int value);

        bool step();
        std::string get_text(int column);
        int get_int(int column);

    private:
        sqlite3_stmt* stmt;
        Database* db;
        bool has_row;
    };

}
