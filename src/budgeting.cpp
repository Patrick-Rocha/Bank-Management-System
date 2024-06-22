/** @brief Grants functions of the budgeting page to the customer
 *
 *  This class represents the budgeting page accessed by the customer, to view metrics such as total spending, and total profit.
 *  @authors Yazan Marwan Alazraq, Rami Istwani, Abdulrehman Khan, You-Chia Kuo, Patrick Rocha
 *  @file budgeting.cpp
 *  @class budgeting "../include/budgeting.h"
 */

#include "budgeting.h"

using namespace std;

/** @brief represents the budgeting page for the customer
 *
 *  Takes a username, and generates the budgeting page for customer associated with the username.
 *  @param username Represents the username of the customer that wants to access their budgeting page
 */
budgeting::budgeting(string username)
{
    this->username = username;

    // Instantiates the data members to 0
    spending = 0.0;
    moneyGained = 0.0;
    initialBalance = 0.0;

    // Opens the database
    rc = sqlite3_open("newDatabase.db", &DB);
}

/** @brief empty constructor for the budgeting object
 *
 *  This method allows for budgeting object instantiation for GUI purposes
 */
budget::budgeting()
{
}

/** @brief destructor for the budgeting object
 *
 *  This method is a destructor that will destroy the budgeting object once it's no longer used.
 */
budgeting::~budgeting()
{
}

/** @brief returns the total amount of spending from the user
 *
 *  This method searches through all of the customer's withdrawal and send transactions, adds up the amount, and returns it to the customer
 *  @return Returns the total amount spent by the user
 */
double budgeting::getSpending()
{
    // Queries the database for the total amount from all withdrawal/send transactions under the specified username
    sql = "SELECT SUM(t.amount) FROM accounts AS a, users AS u, transactions AS t WHERE u.username = a.username"
          " AND a.accountID = t.senderAccountID"
          " AND u.username = \"" +
          username +
          "\" AND (transactionType = \"withdraw\" OR transactionType = \"send\");";

    // Prepares the statement object
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(DB, sql.c_str(), sql.length(), &stmt, nullptr);
    step = sqlite3_step(stmt);

    // Stores the retrieved value
    spending = (sqlite3_column_double(stmt, 0));
    sqlite3_finalize(stmt);

    // Returns the total spend value to the user
    return spending;
}

/** @brief returns the total amount gained for the user
 *
 *  This method searches through all of the customer's deposit and receive transactions, adds up the amount, and returns it to the customer
 *  @return Returns the total amount gained by the user through all accounts
 */
double budgeting::getGained()
{
    // Queries the database for the total amount from all receive/deposit transactions under the specified username.
    sql = "SELECT SUM(t.amount) FROM accounts AS a, users AS u, transactions AS t WHERE u.username = a.username"
          " AND a.accountID = t.senderAccountID"
          " AND u.username = \"" +
          username +
          "\" AND (transactionType = \"deposit\" OR transactionType = \"receive\");";

    // Prepares the statement object.
    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(DB, sql.c_str(), sql.length(), &stmt, nullptr);
    step = sqlite3_step(stmt);

    // Stores the retrieved value.
    moneyGained = (sqlite3_column_double(stmt, 0));
    sqlite3_finalize(stmt);

    // Returns the total gain value.
    return moneyGained;
}

/** @brief returns the total amount of profit for the user
 *
 *  This method searches calculates the total profit by subtracting total spending from total gain
 *  @return Returns the total amount gained by the user through all accounts
 */
double budgeting::getProfit()
{
    return getGained() - getSpending();
}

/** @brief returns the total initial balance from all of the user's accounts
 *
 *  This method adds up the initial balance from all accounts owned by the user, and returns the value.
 *  @return Returns the total initial balance from all accounts owned by the user.
 */
double budgeting::getInitialBalance()
{
    // Queries the database by adding initial balance from all accounts under the specified user
    sql = "SELECT SUM(initialBalance) FROM users AS u, accounts AS a WHERE u.username = a.username"
          " AND u.username = \"" +
          username + "\";";

    // Prepares the statement object.
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(DB, sql.c_str(), sql.length(), &stmt, nullptr);

    step = sqlite3_step(stmt);

    // Stores the retrieved value.
    initialBalance = (sqlite3_column_double(stmt, 0));
    sqlite3_finalize(stmt);

    // Returns the total initial balance
    return initialBalance;
}

/** @brief Helps with executing sql.
 *
 *  This function is used when executing sql statements.
 *  @param unused This variable is not used and serves no purpose.
 *  @param numColumns The number of columns in the table.
 *  @param value List of the values from the current row from each column.
 *  @param columns Name of each column in the table.
 *  @return Always returns to fit the base necessities.
 */
static int callback(void *unused, int numColumns, char **value, char **columns)
{
    for (int i = 0; i < numColumns; i++)
    {
        cout << "Name = " << columns[i] << "   Value = " << value[i] << endl;
    }
    return 0;
}
