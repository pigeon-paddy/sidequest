#include "query.h"
#include "database.h"
#include <stdexcept>

namespace Sidequest::Server {

    Query::Query(Database* db, const std::string& sql)
        : db(db), stmt(nullptr), has_row(false)
    {
        sqlite3* handle = db->get_handle();
        if (sqlite3_prepare_v2(handle, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error("Failed to prepare statement: " + sql);
        }
    }

    Query::~Query() {
        if (stmt) {
            sqlite3_finalize(stmt);
        }
    }

    void Query::bind(int index, const std::string& value) {
        int rc = sqlite3_bind_text(stmt, index, value.c_str(), -1, SQLITE_TRANSIENT);
        if (rc != SQLITE_OK) {
            throw std::runtime_error("Failed to bind string parameter");
        }
    }

    void Query::bind(int index, int value) {
        int rc = sqlite3_bind_int(stmt, index, value);
        if (rc != SQLITE_OK) {
            throw std::runtime_error("Failed to bind int parameter");
        }
    }

    bool Query::step() {
        int rc = sqlite3_step(stmt);
        has_row = (rc == SQLITE_ROW);
        return has_row;
    }

    std::string Query::get_text(int column) {
        const char* text = reinterpret_cast<const char*>(sqlite3_column_text(stmt, column));
        return text ? std::string(text) : "";
    }

    int Query::get_int(int column) {
        return sqlite3_column_int(stmt, column);
    }

}
