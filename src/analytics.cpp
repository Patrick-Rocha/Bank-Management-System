/** @brief Displays analytics
 * 
 *  Offers a variety of information to be viewed that involve the database. This information can only be accessed by administrators for
 *  privacy and security reasons.
 *  @authors Yazan Marwan Alazraq, Rami Istwani, Abdulrehman Khan, You-Chia Kuo, Patrick Rocha
 *  @file analytics.cpp
 *  @class analytics "../include/analytics.h"
 * 
*/  

#include "analytics.h"
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
 *  Constructor that opens the database used to obtain information from.
*/
analytics::analytics() {
    errorMessage = 0;
	// opens the database file, returns an error if it fails
    rc = sqlite3_open("bankDatabase.db", &db);
    if (rc) {
        cout << "Can't open database" << endl;
	}
}

/** @brief Checks if a user exists. 
 *  @param username The username used to perform the check.
 *  @return Returns true if the username does exist, and false otherwise.
 *  
 *  Searches through the users table in the database for the given username.
*/
bool analytics::userExists(string username) {
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

/** @brief Gets the number of users
 *  @return The number of users.
 * 
 *  Goes through the users table in the database and checks how many rows (users) there are.
*/
int analytics::getNumUsers() {
    sql = "SELECT COUNT(*) FROM users WHERE userType=\"regular\";";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &stmt, nullptr);
    step = sqlite3_step(stmt);
    int numUsers = (sqlite3_column_int(stmt, 0));
    sqlite3_finalize(stmt);
    return numUsers;
}

/** @brief Gets the balance of a user.
 *  @param username The username of the user we wish to check the balance of.
 *  @return The total balance of the user.
 * 
 * Goes through all of the given user's accounts and totals up the balance.
*/
double analytics::getBalance(string username) {
    sqlite3_stmt* stmt;
    if (userExists(username)) {
        double totalBalance = 0;
        sql = "SELECT COUNT(*) FROM accounts WHERE username = \"" + username + "\");";
        sql = "SELECT EXISTS(SELECT 1 FROM accounts WHERE username = \"" + username + "\");";
        sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &stmt, nullptr);
        step = sqlite3_step(stmt);
        int empty = (sqlite3_column_int(stmt,0));
        sqlite3_finalize(stmt);
        if (empty == 0) {
            return 0.00;
        }

        sql = "SELECT balance FROM accounts WHERE username = \"" + username + "\";";
        sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &stmt, nullptr);
        step = sqlite3_step(stmt);
        while (step == 100) {
            totalBalance += (sqlite3_column_double(stmt,0));
            step = sqlite3_step(stmt);
        }
        sqlite3_finalize(stmt);
        return totalBalance;
    }
    return -1;
}

/** @brief Calculates the average balance.
 *  
 *  Looks at every user's total balance and calculates the average of those numbers.
*/  
void analytics::calculateAverageBalance() {
    string username = "";
    sql = "SELECT username FROM users WHERE userType = \"regular\";";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &stmt, nullptr);
    step = sqlite3_step(stmt);
    averageBalance = 0;
    while (step == SQLITE_ROW) {
        username = (reinterpret_cast<char const*>(sqlite3_column_text(stmt,0)));
        averageBalance += this->getBalance(username);
        step = sqlite3_step(stmt);
    }
    sqlite3_finalize(stmt);
    averageBalance = averageBalance / this->getNumUsers();
}

/** @brief Calculates the average credit score.
 * 
 *  Totals the credit score of all regular users, then divides by the number of users to get the average credit score.
*/
void analytics::calculateAverageCreditScore() {
    sql = "SELECT creditScore FROM users WHERE userType = \"regular\";";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &stmt, nullptr);
    step = sqlite3_step(stmt);
    averageCredit = 0;
    while (step == 100) {
        averageCredit += (sqlite3_column_int(stmt, 0));
        step = sqlite3_step(stmt);
    }
    sqlite3_finalize(stmt);
    averageCredit = averageCredit / this->getNumUsers();
}

/** @brief Gets the average balance.
 *  @return The average balance between all users.
 * 
 *  Getter function to give the average balance between all users rounded to two decimal places.
*/
double analytics::getAverageBalance() {
    calculateAverageBalance();
    return floor(averageBalance * 100.00) / 100.00;
}

/** @brief Gets the average credit score.
 *  @return The average credit score between all users. 
 *  
 *  Getter function to give the average credit between all users.
*/
double analytics::getAverageCreditScore() {
    calculateAverageCreditScore();
    return averageCredit;
}

/** @brief Gets the number of transactions.
 *  @return The total number of transactions.
 * 
 *  Looks through the transactions table in the database and counts the number of rows, equal to the number of transactions. 
*/
int analytics::getNumTransactions() {
    sql = "SELECT COUNT(*) FROM transactions;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &stmt, nullptr);
    step = sqlite3_step(stmt);
    totalTransactions = sqlite3_column_double(stmt,0);
    sqlite3_finalize(stmt);
    return totalTransactions;
}

/** @brief Gets the credit score of a user.
 *  @param username The username of the user we are getting the credit score from.
 *  @return The credit score of the user, or -1 if the user does not exist.
 * 
 *  Looks through the users table in the database for the user's credit score given their username.
*/
int analytics::getCreditScore(string username) {
    sqlite3_stmt* stmt;
    if (userExists(username)) {
        sql = "SELECT creditScore FROM users WHERE username = \"" + username + "\";";
        sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &stmt, nullptr);
        step = sqlite3_step(stmt);
        double creditScore = (sqlite3_column_int(stmt,0));
        sqlite3_finalize(stmt);
        return creditScore;
    }
    return -1;
}
