#include "database.h"

void Storage::init_db() {
    const char* init_sql = R"(
        PRAGMA foreign_keys = ON;
        PRAGMA journal_mode = WAL;
    )";
    sqlite3_exec(db, init_sql, nullptr, nullptr, nullptr);

    const char* users_sql = R"(
        CREATE TABLE IF NOT EXISTS users (
            user_id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE NOT NULL,
            is_admin INTEGER DEFAULT 0
        );
    )";


    const char* chats_sql = R"(
        CREATE TABLE IF NOT EXISTS chats (
            chat_id INTEGER PRIMARY KEY AUTOINCREMENT,
            chat_data JSON NOT NULL DEFAULT '{
                "title": "",
                "participants": [],
                "messages": []
            }',
            last_updated TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        );
        
        CREATE INDEX IF NOT EXISTS idx_chats_updated ON chats(last_updated);
    )";
    const char* sql_commands[] = {users_sql, chats_sql};
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

void Storage::add_message(int chat_id, int sender_id, const string text){
    const char* sql = R"(
        UPDATE chats 
        SET chat_data = json_set(
            chat_data,
            '$.messages', 
            json_insert(
                json_extract(chat_data, '$.messages'),
                '$[#]',
                json_object(
                    'id', json_array_length(json_extract(chat_data, '$.messages')) + 1,
                    'sender', ?2,
                    'text', ?3,
                    'timestamp', datetime('now')
                )
            )
        )
        WHERE chat_id = ?1;
    )";

    sqlite3_stmt* stmt;
    int res = sqlite3_prepare_v2(db,sql,-1,&stmt,0);

    sqlite3_bind_int(stmt,1,chat_id);
    sqlite3_bind_int(stmt,2,sender_id);
    sqlite3_bind_text(stmt,3,text.c_str(),-1,SQLITE_TRANSIENT);

    res = sqlite3_step(stmt);


    sqlite3_finalize(stmt);

}