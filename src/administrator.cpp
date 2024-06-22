/** @brief Grants administrator functions.
 * 
 *  Allows the administrator, a user with more privilege, to perform various administrator-exclusive functions. This is especially in 
 *  regards to editing users and viewing user information.
 *  @authors Yazan Marwan Alazraq, Rami Istwani, Abdulrehman Khan, You-Chia Kuo, Patrick Rocha
 *  @file administrator.cpp
 *  @class administrator "../include/administrator.h"
*/  

#include "administrator.h"

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

/** @brief Opens the database.
 * 
 * Opens the bank database and allows the database's foreign keys to be usable.
*/
administrator::administrator() {
    rc = sqlite3_open("bankDatabase.db", &db);
    if (rc) {
        cout << "Can't open database" << endl;
	}

    //Allowing the compatibility of foreign keys
    sql = "PRAGMA foreign_keys = ON;";
    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &errorMessage);
}

/** @brief Checks if a user exists.
 *  @param username We are checking if this username exists in the database.
 *  @return Returns true if the username does exist in the database, and false otherwise.
 * 
 * Taking in a username, this function goes through every username in the users table to find a match.
*/
bool administrator::userExists(string username) {
    sql = "SELECT EXISTS(SELECT 1 FROM users WHERE username = \"" + username + "\");";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &stmt, nullptr);
    step = sqlite3_step(stmt);
    empty = (sqlite3_column_int(stmt,0));
    sqlite3_finalize(stmt);
    if (empty == 0) {
        return false;
    }
    return true;
}

/** @brief Checks if an account exists.
 *  @param accountID We are checking if this account ID exists in the database.
 *  @return Returns true if the account ID does exist in the database, and false otherwise.
 * 
 * Taking in an account ID, this function goes through every account ID in the accounts table to find a match.
*/
bool administrator::accountExists(int accountID) {
    sql = "SELECT EXISTS(SELECT 1 FROM accounts WHERE accountID = " + to_string(accountID) + ";";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &stmt, nullptr);
    step = sqlite3_step(stmt);
    empty = (sqlite3_column_int(stmt,0));
    sqlite3_finalize(stmt);
    if (empty == 0) {
        return false;
    }
    return true;
}

/** @brief Gets the name of a given user.
 *  @param username The username we are getting the name for.
 *  @return Returns the name of the user, or an empty string if the user does not exist.
 * 
 *  Searches through the database to find the name of a user with a given username.
*/
string administrator::getName(string username) {
    sqlite3_stmt* stmt;
    if (userExists(username)) {
        sql = "SELECT name FROM users WHERE username = \"" + username + "\";";
        sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &stmt, nullptr);
        step = sqlite3_step(stmt);
        user = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        sqlite3_finalize(stmt);
        return user;
    }
    return "";
}

/** @brief Gets the credit score of a given user.
 *  @param username The username we are getting the credit score for.
 *  @return Returns the credit score of the user, or -1 if the user does not exist.
 * 
 *  Searches through the database to find the credit score of a user with a given username.
*/
int administrator::getUserCreditScore(string username) {
    sqlite3_stmt* stmt;
    if (userExists(username)) {
        sql = "SELECT creditScore FROM users WHERE username = \"" + username + "\";";
        sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &stmt, nullptr);
        step = sqlite3_step(stmt);
        int creditScore = (sqlite3_column_int(stmt, 0));
        sqlite3_finalize(stmt);
        return creditScore;
    }
    return -1;
}  

/** @brief Gets the loan debt of a given user.
 *  @param username The username we are getting the loan debt for.
 *  @return Returns the loan debt of the user, or -1 if the user does not exist.
 * 
 *  Searches through the database to find the loan debt of a user with a given username.
*/
double administrator::getUserLoanDebt(string username) {
    sqlite3_stmt* stmt;
    if (userExists(username)) {
        sql = "SELECT loanDebt FROM users WHERE username = \"" + username + "\";";
        sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &stmt, nullptr);
        step = sqlite3_step(stmt);
        double loanDebt = (sqlite3_column_double(stmt, 0));
        sqlite3_finalize(stmt);
        return loanDebt;
    }
    return -1;
}

/** @brief Gets the user type of a given user.
 *  @param username The username we are getting the user type for.
 *  @return Returns the user type of the user, or an empty string if the user does not exist.
 * 
 *  Searches through the database to find if a user is a "regular" customer or an "admin".
*/
string administrator::getUserType(string username) {
    sqlite3_stmt* stmt;
    if (userExists(username)) {
        sql = "SELECT userType FROM users WHERE username = \"" + username + "\";";
        sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &stmt, nullptr);
        step = sqlite3_step(stmt);
        string userType(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        sqlite3_finalize(stmt);
        return userType;
    }
    return "";
}

/** @brief Updates the credit score of a user.
 *  @param username The ID of the user we want to edit the credit score for.
 *  @param amount The new credit score we wish to give to a user.
 * 
 *  Given a user, updates their credit score to a new value.
*/
void administrator::updateCreditScore(string username, int amount) {
    if (userExists(username)) {
        sql = "UPDATE users SET creditScore = " + to_string(amount) + " where username = \"" + username + "\";";
        rc = sqlite3_exec(db, sql.c_str(), callback, 0, &errorMessage);
    }
}

/** @brief Removes a user.
 *  @param username The username we are getting the credit score for.
 * 
 *  Deletes a user from the database if it exists.
*/
void administrator::removeUser(string username) {
    if (userExists(username)) {
        sql = "DELETE FROM users WHERE username = \"" + username + "\";";
        rc = sqlite3_exec(db, sql.c_str(), callback, 0, &errorMessage);
    } else {
        cout << "USER DOESN'T EXIST" << endl;
    }
}

/** @brief Grants a loan to a user.
 *  @param accountID The unique ID of the user's account we wish to add money to.
 *  @param amount The amount of money we wish to add to the account.
 * 
 *  Adds money to a user's account while also increasing their loan debt by the same amount.
*/
void administrator::giveLoan(int accountID, double amount) {
    sqlite3_stmt* stmt;
    if (accountExists(accountID)) {
        // Adding to the balance of the account.
        sql = "update accounts set balance = balance + " + to_string(amount) + " where accountID = " + to_string(accountID) + ";";
        rc = sqlite3_exec(db, sql.c_str(), callback, 0, &errorMessage);

        // Finding the owner of the account
        sql = "select username from accounts where accountID = " + to_string(accountID) + ";";
        sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &stmt, nullptr);
        step = sqlite3_step(stmt);
        string username(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        sqlite3_finalize(stmt);

        // Increasing the loan debt on the account
        sql = "update users set loanDebt = loanDebt + " + to_string(amount) + " where username = " + username + ";";
        rc = sqlite3_exec(db, sql.c_str(), callback, 0, &errorMessage);
    }
}

/** @brief Creates a user.
 *  @param name The real name of the user.
 *  @param username The unique username of the user.
 *  @param password The user's password needed to login.
 * 
 *  Adds an extra row to the users table in the database if the given username doesn't already exist.
*/
void administrator::createUser(string name, string username, string password) {
    if (!userExists(username)) {
        sql = "insert or ignore into users " \
        "(username, password, name, userType) values " \
        "(\"" + username + "\",\"" + password + "\",\"" + name + "\",\"regular\");";
        rc = sqlite3_exec(db, sql.c_str(), callback, 0, &errorMessage);
    }
}
