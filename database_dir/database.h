#include <sqlite3.h>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;
using namespace std;


class Storage{
    public:
        Storage();
        Storage(const string db_path){
            if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK){
                cerr << "Error while opening database";
                db = nullptr;
            }else{
                cout << "Database opened";
                init_db();
            }
        }

        ~Storage(){
            if (db){
                sqlite3_close(db);
                cout << "Database closed";
            }
        }

        void init_db();

        void add_user(string name);

        void add_message(int chat_id, int sender_id, const string text);

    private:
        sqlite3* db;
};