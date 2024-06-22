/** @brief Lets a user login to the bank.
 * 
 *  Holds the database that contains all user's information. When attempting to login, this database is used to
 *  verify if the credentials are valid.
 *  @authors Yazan Marwan Alazraq, Rami Istwani, Abdulrehman Khan, You-Chia Kuo, Patrick Rocha
 *  @file login.cpp
 *  @class login "../include/login.h"
*/  

#include "login.h"

using namespace std;

/** @brief Helps with executing sql.
 *  @param unused This variable is not used and serves no purpose.
 *  @param numColumns The number of columns in the table.
 *  @param value List of the values from the current row from each column.
 *  @param columns Name of each column in the table.
 *  @return Always returns to fit the base necessities.
 * 
 *  This function is used when executing sql statements.
*/  
static int callback(void *unused, int numColumns, char **value, char **columns) {
    for (int i = 0; i < numColumns; i++) {
        cout << "Name = " << columns[i] << "   Value = " << value[i] << endl;
        }
    return 0;
}

/** @brief Creats the bank's database.
 *  
 *  Creates an sqlite database for the bank including a users table, accounts table, and transactions table.
*/
login::login() {
    errorMessage = 0;
	// opens the database file, returns an error if it fails
    rc = sqlite3_open("bankDatabase.db", &db);
    if (rc) {
        cout << "Can't open database" << endl;
	}

    //Allowing the compatibility of foreign keys
    sql = "PRAGMA foreign_keys = ON;";
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &errorMessage);

	// Creates the user table
    sql = "create table if not exists users (" \
    "username varchar(20) PRIMARY KEY, " \
    "password varchar(20) NOT NULL, " \
    "name varchar(20), " \
    "creditScore INTEGER DEFAULT 300, " \
    "loanDebt decimal(15,2) DEFAULT 0, " \
    "userType varchar(10) NOT NULL);";
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &errorMessage);

    // Creates the accounts table
    sql = "create table if not exists accounts (" \
    "accountID INTEGER PRIMARY KEY AUTOINCREMENT, " \
    "username varchar(20), " \
    "accountType varchar(15) NOT NULL, " \
    "initialBalance decimal(15,2), " \
    "balance decimal(15,2), " \
    "FOREIGN KEY (username) REFERENCES users(username) ON DELETE CASCADE);";
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &errorMessage);

    // Creates the transactions table
    sql = "create table if not exists transactions (" \
    "transactionID INTEGER PRIMARY KEY AUTOINCREMENT, " \
    "senderAccountID INTEGER, " \
    "receiverAccountID INTEGER, " \
    "transactionType varchar(10) NOT NULL, " \
    "amount decimal(15,2) NOT NULL, " \
    "transactionTime DATETIME default CURRENT_TIMESTAMP NOT NULL, " \
    "FOREIGN KEY(senderAccountID) REFERENCES accounts(accountID) ON DELETE CASCADE);";
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &errorMessage);

	// Inserting four users including one admin into the users table
    sql = "insert or ignore into users " \
    "(username, password, name, userType) values " \
    "(\"admin001\",\"adminpassword\",\"bankAdmin\",\"admin\")," \
	"(\"user001\",\"oneuser\",\"bob\",\"regular\")," \
	"(\"user002\",\"twouser\",\"jannet\",\"regular\")," \
	"(\"user003\",\"threeuser\",\"sarah\",\"regular\");";
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &errorMessage);

    // Inserting three accounts into the accounts table
    sql = "insert or ignore into accounts " \
    "(username, accountType, initialBalance, balance) values " \
    "(\"user001\",\"chequing\",1234.22,1234.55), " \
    "(\"user001\",\"savings\",12134.22,12434.55), " \
    "(\"user003\",\"savings\",130.60,654.89);";
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &errorMessage);
}

/** @brief Checks if the user can login with their information.
 *  @param username The inserted username that will be checked in the database.
 *  @param password The inserted password that will be checked in the database.
 *  @return Returns true if the user can login with their inserted information, and false otherwise.
 * 
 *  Takes a username and password, and searchs the user table for a row that contains both the given username and password.
*/
bool login::verifyLogin(string username, string password) {
    accountFound = false;
    sql = "SELECT username, password FROM users;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &stmt, nullptr);
    step = sqlite3_step(stmt);

    // Looping through each row
    while (step == 100) {
        string user(reinterpret_cast<char const*>(sqlite3_column_text(stmt, 0)));
        string pass(reinterpret_cast<char const*>(sqlite3_column_text(stmt, 1)));
        if (username == user && password == pass) {
            accountFound = true;
        }
        step = sqlite3_step(stmt);
    }
    sqlite3_finalize(stmt);
	return accountFound;
}

/** @brief Checks the user type.
 *  @param username The username of the user where we will check the user type for.
 *  @return The user type of the user, which is either "regular" or "admin".
 *  
 *  Takes a username, searches the users table for the user's account type, and returns it.
*/
string login::checkUserType(string username) {
	sqlite3_stmt* stmt;
	string command = "SELECT userType FROM users WHERE username = \"" + username + "\";";
	sqlite3_prepare_v2(db, command.c_str(), -1, &stmt, 0);
	sqlite3_step(stmt);
	string result = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
	return result;
}
