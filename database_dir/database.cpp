#include "database.h"

void Storage::init_db() {
    const char* init_sql = R"(
        PRAGMA journal_mode = WAL;
    )";
    sqlite3_exec(db, init_sql, nullptr, nullptr, nullptr);

    const char* users_sql = R"(
        CREATE TABLE IF NOT EXISTS users (
            user_id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE NOT NULL
        );
    )";

    const char* messages_sql = R"(
        CREATE TABLE IF NOT EXISTS messages (
            message_id INTEGER PRIMARY KEY AUTOINCREMENT,
            chat_id INTEGER NOT NULL,
            sender_id INTEGER NOT NULL,
            text TEXT NOT NULL,
            timestamp DATETIME DEFAULT CURRENT_TIMESTAMP
        );
        
        CREATE INDEX IF NOT EXISTS idx_messages_chat ON messages(chat_id);
    )";

    const char* sql_commands[] = {users_sql, messages_sql};
    char* err_msg = nullptr;
    
    for (const char* sql : sql_commands) {
        if (sqlite3_exec(db, sql, nullptr, nullptr, &err_msg) != SQLITE_OK) {
            std::cerr << "SQL error: " << err_msg << std::endl;
            sqlite3_free(err_msg);
            throw std::runtime_error("Database initialization failed");
        }
    }
}
void Storage::add_user(string name){
    const char* sql = "INSERT INTO users (username) VALUES (?)";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db,sql,-1,&stmt,0);

    if (rc != SQLITE_OK){
        cerr << "Prepare faild" << endl;
        return;
    }

    rc = sqlite3_bind_text(stmt,1,name.c_str(),-1,SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE){
        cerr << "Insert faild" << endl;
        return;
    }

    sqlite3_finalize(stmt);

}

bool Storage::add_message(int chat_id, int sender_id, const string text) {
    const char* sql = R"(
        INSERT INTO messages (chat_id, sender_id, text)
        VALUES (?, ?, ?)
    )";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, chat_id);
    sqlite3_bind_int(stmt, 2, sender_id);
    sqlite3_bind_text(stmt, 3, text.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

vector<Message> Storage::get_messages(int chat_id, int limit = 100) {
    const char* sql = R"(
        SELECT m.message_id, m.sender_id, m.text, m.timestamp
        FROM messages m
        WHERE m.chat_id = ?
        ORDER BY m.timestamp DESC
        LIMIT ?
    )";

    vector<Message> messages;
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return messages;
    }

    sqlite3_bind_int(stmt, 1, chat_id);
    sqlite3_bind_int(stmt, 2, limit);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Message msg;
        msg.id = sqlite3_column_int(stmt, 0);
        msg.sender_id = sqlite3_column_int(stmt, 1);
        msg.text = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        msg.timestamp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        messages.push_back(msg);
    }

    sqlite3_finalize(stmt);
    return messages;
}

void Storage::delete_messages(){
    char* sql = "DELETE FROM messages";
    char* err_mess;

    int res = sqlite3_exec(db,sql,nullptr,nullptr,&err_mess);
}

void Storage::delete_users(){
    char* sql = "DELETE FROM users";
    char* err_mess;

    int res = sqlite3_exec(db,sql,nullptr,nullptr,&err_mess);
}