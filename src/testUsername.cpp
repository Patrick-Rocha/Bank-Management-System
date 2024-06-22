#include "sqlite3.h"
#include <string>
#include <iostream>

using namespace std;

int main() {
    //string administrator::getUsername(string username) {
    char *errorMessage = 0;
    sqlite3 *db;
	// opens the database file, returns an error if it fails
    int rc = sqlite3_open("bankDatabase.db", &db);
    if (rc) {
        cout << "Can't open database" << endl;
	}
    
    string sql = "SELECT name FROM users WHERE username = \"user001\";";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &stmt, nullptr);
    int step = sqlite3_step(stmt);
    string user = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    sqlite3_finalize(stmt);
    cout << user << endl;
}
