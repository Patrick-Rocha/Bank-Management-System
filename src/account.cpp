/** @brief Grants account functions.
 *
 *  This class represents an account record in the accounts table, and allows the user to create an account, or fetch account data from the accounts *  table in the database.
 *  @authors Yazan Marwan Alazraq, Rami Istwani, Abdulrehman Khan, You-Chia Kuo, Patrick Rocha
 *  @file account.cpp
 *  @class account "../include/account.h"
 *
 */

#include "account.h"

using namespace std;

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

/** @brief Creates a new account for the customer.
 *
 *  Takes an accountType, username, and initial deposit. This will populate the account's data members, and then
 *  Writes the account to the accounts table in the database
 *  @param accountType Represents the type of account to be opened
 *  @param username Represents the username of the customer that's opening the account
 *  @param smoney Represents the initial deposit for the account upon opening.
 */
account::account(string accountType, string username, double smoney)
{
	// Populates data members
	this->accountType = accountType;
	this->username = username;
	balance = smoney;
	errorMessage = 0;

	// Opens the database
	rc = sqlite3_open("newDatabase.db", &DB);

	// Adds a new row to the accounts table, with the parameter values provided.
	sql = "INSERT INTO accounts(username, accountType, initialBalance, balance) VALUES (\"" + username + "\", \"" + accountType + "\", " + to_string(smoney) + ", " + to_string(smoney) + ");";

	rc = sqlite3_exec(DB, sql.c_str(), callback, 0, &errorMessage);

	storeValues();
}

/** @brief Creates an account object to represent an EXISTING account.
 *
 *  Creates an account object to represent an existing account. This constructor will not write to the database, but instead
 *  fetch it's data from the accounts table and populates its data members.
 *  @param accountType Represents the type of account
 *  @param username Represents the username of the customer that owns this account
 *
 */
account::account(string accountType, string username)
{
	// Populates data members
	this->accountType = accountType;
	this->username = username;
	errorMessage = 0;

	rc = sqlite3_open("newDatabase.db", &DB);

	// fetches account data from the accounts table and stores it.
	storeValues();
}

/** @brief destructor for the account object
 *
 *  This method is a destructor that will destroy the account object once it's no longer used.
 */
account::~account()
{
}

/** @brief Returns the balance of this account
 *
 *  This method first fetches the balance in the database, stores it in balance, and returns its value
 *	@return returns the balance of the account
 *
 */
double account::getBalance()
{
	// Store the most up-to-date balance value of the account.
	refreshBalance();
	return balance;
}

/** @brief Creates an account object to represent an EXISTING account.
 *
 *	This method checks the account's balance, to see if a loan is approved
 *  @param amount Represents the amount requested
 *	@return returns true if balance is greater than 500, false otherwise.
 *
 */
bool account::applyForLoan(double amount)
{
	// Store the most up-to-date balance value of the account.
	refreshBalance();

	// If the account has over $500 in funds, approves the loan
	if (balance > 500.0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/** @brief Returns the account ID
 *
 *	This method returns the account's ID to the customer
 *	@return returns accountID, which represents the accountID associated with the account.
 *
 */
int account::getID()
{
	return accountID;
}

/** @brief Returns the username associated with the account
 *
 *	This method returns the username associated with the account
 *	@return returns username, which represents the username associated with the account.
 *
 */
string account::getUserName()
{
	return username;
}

/** @brief Returns the account type of the account
 *
 *	This method returns the account type, whether it be a chequing or saving account
 *	@return returns accountType, which represents the account type of this account.
 *
 */
string account::getAccountType()
{
	return accountType;
}

/** @brief Withdraw money from the customer account
 *
 *	This method will check to see if the account has sufficient funds. If so, it writes the transaction to the database, subtracts the requested
 *  amount from the account, and stores the new value in the data member
 *  @param amount Represents the amount to be withdrawn
 *  @return returns true if the account has enough funds to be withdrawn. False otherwise.
 *
 */
bool account::withdraw(double amount)
{
	// Store the most up-to-date balance value of the account.
	refreshBalance();

	// If the account has enough funds remaining, updates the transactions table, and the account balance
	if (balance >= amount)
	{
		sql = "INSERT INTO transactions(senderAccountID, transactionType, amount) VALUES (" + to_string(accountID) + ", \"withdraw\", " + to_string(amount) + ");";
		rc = sqlite3_exec(DB, sql.c_str(), callback, 0, &errorMessage);

		sql = "UPDATE accounts SET balance = balance - " + to_string(amount) + " WHERE accountID = " + to_string(accountID) + ";";
		rc = sqlite3_exec(DB, sql.c_str(), callback, 0, &errorMessage);

		// Stores the new balance in the account object
		refreshBalance();
		return true;
	}
	else
	{
		cout << "Not Enough Funds!" << endl;
		return false;
	}
}

/** @brief Deposit money into the customer account
 *
 *	This method will add the transaction record into the transaction table, and adds the amount to the balance in the accounts table.
 *	It then pulls the new balance from the database and stores it into the object.
 *  @param amount Represents the amount to deposit
 *  @return returns true if the account successfully deposits the money
 *
 */
bool account::deposit(double amount)
{
	// Updates the transactions table, and the account balance
	sql = "INSERT INTO transactions(senderAccountID, transactionType, amount) VALUES (" + to_string(accountID) + ", \"deposit\", " + to_string(amount) + ");";
	rc = sqlite3_exec(DB, sql.c_str(), callback, 0, &errorMessage);

	sql = "UPDATE accounts SET balance = balance + " + to_string(amount) + " WHERE accountID = " + to_string(accountID) + ";";
	rc = sqlite3_exec(DB, sql.c_str(), callback, 0, &errorMessage);

	// Stores the new balance in the account object
	refreshBalance();
	return true;
}

/** @brief Fetches the account information from the database
 *
 *	This method fetches the most updated accountType, balance, and accountID information from the accounts table, and stores it in
 *  this account object.
 */
void account::storeValues()
{
	// Declares the statement object
	sqlite3_stmt *stmt;

	// Retrieves the account's type, balance, and ID
	sql = "SELECT accountType, balance, accountID FROM accounts WHERE username = \"" + username + "\" AND accountType = \"" + accountType + "\";";
	sqlite3_prepare_v2(DB, sql.c_str(), sql.length(), &stmt, nullptr);

	step = sqlite3_step(stmt);

	// Parses the returned row, and stores each value in its respective data members
	accountType = string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
	balance = (sqlite3_column_double(stmt, 1));
	accountID = (sqlite3_column_int(stmt, 2));

	// Destroys the statement object
	sqlite3_finalize(stmt);
}

/** @brief Refreshes the balance of the account
 *
 *	This method fetches the most updated accountType, balance, and accountID information from the accounts table, and stores it in
 *  this account object.
 */
void account::refreshBalance()
{
	// Declares the statement object
	sqlite3_stmt *stmt;

	// Retrieves the account's balance
	sql = "SELECT balance FROM accounts WHERE username = \"" + username + "\" AND accountType = \"" + accountType + "\";";

	sqlite3_prepare_v2(DB, sql.c_str(), sql.length(), &stmt, nullptr);

	step = sqlite3_step(stmt);

	// Stores the balance
	balance = (sqlite3_column_double(stmt, 0));

	// Destroys the statement object
	sqlite3_finalize(stmt);
}
