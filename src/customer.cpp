/** @brief Grants customer functions.
 *
 *  This class represents a customer using the banking system. The customer can fetch their credit score, the total amount of money in their accounts,
 *  send money to another account, or create/delete an account.
 *  @authors Yazan Marwan Alazraq, Rami Istwani, Abdulrehman Khan, You-Chia Kuo, Patrick Rocha
 *  @file customer.cpp
 *  @class customer "../include/customer.h"
 */

#include "customer.h"

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

/** @brief Opens the database and fetches all existing accounts
 *
 *  Takes in a username, opens the database, and stores all accounts under the user into the accounts vector
 *
 *  @param username Represents the username of the regular user
 */
customer::customer(string username)
{
	this->username = username;

	rc = sqlite3_open("newDatabase.db", &DB);

	// Stores a list of all account types under the user.
	vector<string> accountTypes;

	// Runs a query to get the account types of the user, and for each one, stores the account type in the accountType vector.
	sqlite3_stmt *stmt;
	sql = "SELECT accountType FROM accounts WHERE username = \"" + username + "\";";
	sqlite3_prepare_v2(DB, sql.c_str(), sql.length(), &stmt, nullptr);

	while (sqlite3_step(stmt) != SQLITE_DONE)
	{
		string fetchedAccountType = string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
		accountTypes.push_back(fetchedAccountType);
	}

	// For each account type the user possesses, creates the account object, and adds it to the account list.
	if (accountTypes.size() > 0)
	{
		for (int i = 0; i < accountTypes.size(); i++)
		{
			account temp(accountTypes[i], username);
			accounts.push_back(temp);
		}
	}

	sqlite3_finalize(stmt);

	// fetches the user's data and stores it.
	storeValues();
}

/** @brief destructor for the customer object
 *
 *  This method is a destructor that will destroy the customer object onece it's no longer used.
 *
 */
customer::~customer()
{
}

/** @brief Returns the credit score of the user
 *
 *  This method returns the user's credit score.
 *  @return returns the user's credit score
 *
 */
int customer::getCreditScore()
{
	return creditScore;
}

/** @brief Returns the total amount of money from all accounts
 *
 *  This method iterates through the user's list of accounts, takes the balance from each one, and adds it to the money variable, which is then
 *  returned to the user
 *  @return returns total money the user has across all accounts
 *
 */
double customer::getMoney()
{
	money = 0;

	// Iterates through the user's account list, and adds their balances to money.
	for (int i = 0; i < accounts.size(); i++)
	{
		money += accounts[i].getBalance();
	}
	return money;
}

/** @brief Returns the granted loan debt of the user
 *
 *  This method returns the user's amount of loan debt.
 *  @return returns the total loan debt of the user
 *
 */
double customer::getLoanDebt()
{
	return loanDebt;
}

/** @brief returns the balance of the user account
 *
 *  This method takes in an account type, looks for the specified account,
 *  and returns the balance of that account. If it doesn't exist, it returns 0.
 *  @param accountType Represents which account type the user is fetching the balance from
 *  @return returns the account balance from the account of the specified account type.
 *
 */
double customer::checkAccountBalance(string accountType)
{

	// Iterates through the accounts list to find an account with the specified account type.
	for (int i = 0; i < accounts.size(); i++)
	{
		// If there is a match, returns the balance of that account.
		if (accounts[i].getAccountType().compare(accountType) == 0)
		{
			return accounts[i].getBalance();
		}
	}
	return 0;
}

/** @brief creates a new account for the customer
 *
 *  This method takes in an account type and initial deposit value. It searches the user's account list to make sure there are no other accounts of *  the same type. If so, it creates a new account with that type, and adds it to the user's account list
 *  @param accountType Represents the type of account the customer wants to open
 *  @param smoney Represents the initial deposit into the new account
 *  @return returns total money the user has across all accounts
 *
 */
bool customer::createAccount(string accountType, double smoney)
{
	// Iterates through the accounts list to see if the account type already exists
	for (int i = 0; i < accounts.size(); i++)
	{
		// If so, return false
		if (accounts[i].getAccountType().compare(accountType) == 0)
		{
			return false;
		}
	}

	// Otherwise, create the new account, and add it to the account list.
	account newAccount(accountType, username, smoney);
	accounts.push_back(newAccount);

	return true;
}

/** @brief deletes a customer account
 *
 *  This method takes in an account type. It searches the user's account list for the specified account, and if found, deletes its record
 *  from the accounts table, and deletes all transactions associated with that account.
 *  Returns true upon success, and false if the account wasn't found.
 *  @param accountType Represents the type of account the customer wants to delete
 *  @return returns true if the account exists and is deleted, false otherwise.
 *
 */
bool customer::deleteAccount(string accountType)
{
	char *errorMessage = 0;

	// Iterates through the accounts list to find the specified account
	for (int i = 0; i < accounts.size(); i++)
	{
		// If it exists, deletes the account's records from accounts, as well as its transactions from the transactions table.
		if (accounts[i].getAccountType().compare(accountType) == 0)
		{
			sql = "DELETE FROM transactions WHERE senderAccountID = " + to_string(accounts[i].getID()) + ";";
			rc = sqlite3_exec(DB, sql.c_str(), callback, 0, &errorMessage);

			sql = "DELETE FROM accounts WHERE username = \"" + username + "\" AND accountType = \"" + accountType + "\";";
			rc = sqlite3_exec(DB, sql.c_str(), callback, 0, &errorMessage);

			// Removes the account from the accounts list.
			accounts.erase(accounts.begin() + i);

			return true;
		}
	}
	return false;
}

/** @brief sends money to another account
 *
 *  This method takes in an account type. It searches the user's account list for the specified account, and if found, deletes its record
 *  from the accounts table, and deletes all transactions associated with that account.
 *  @param senderAccountID Represents sender account ID
 *  @param receiverAccountID Represents receiver account ID
 *  @param amount Represents the amount of the money the customer wants to send
 *  @return returns true if the user has enough funds, the sender/receiver account ID exists, and the SQL statements run successfully,
 *  false otherwise.
 */
bool customer::transaction(int senderAccountID, int receiverAccountID, double amount)
{
	sqlite3_stmt *stmt;
	int idExists = 0;	// flag to track if the specified receiver account exists.
	double balance = 0; // stores the balance of the sender account

	// Iterates through the accounts list, looks for the sender account, and compares its balance with the user amount
	for (int i = 0; i < accounts.size(); i++)
	{
		if (accounts[i].getID() == senderAccountID)
		{
			balance = accounts[i].getBalance();

			// Returns false if the user doesn't have enough funds in the account.
			if (balance < amount)
			{
				cout << "Not enough funds remaining." << endl;
				return false;
			}
		}
	}

	// Queries the database to see if the receiver account exists.
	sql = "SELECT COUNT(*) FROM accounts WHERE accountID = " + to_string(receiverAccountID) + ";";
	sqlite3_prepare_v2(DB, sql.c_str(), sql.length(), &stmt, nullptr);
	step = sqlite3_step(stmt);

	idExists = (sqlite3_column_int(stmt, 0));

	sqlite3_finalize(stmt);

	// If the account exists, sends money to the receiver account, while updating account balances on both ends, and adding the transaction
	// to the transactions table
	if (idExists == 1)
	{
		char *errorMessage = 0;

		sql = "INSERT INTO transactions(senderAccountID, receiverAccountID, transactionType, amount) VALUES (" + to_string(senderAccountID) + ", " + to_string(receiverAccountID) + ", \"send\", " + to_string(amount) + ");";
		rc = sqlite3_exec(DB, sql.c_str(), callback, 0, &errorMessage);

		sql = "UPDATE accounts SET balance = balance - " + to_string(amount) + " WHERE accountID = " + to_string(senderAccountID) + ";";
		rc = sqlite3_exec(DB, sql.c_str(), callback, 0, &errorMessage);

		sql = "INSERT INTO transactions(senderAccountID, transactionType, amount) VALUES (" + to_string(receiverAccountID) + ", \"receive\", " + to_string(amount) + ");";
		rc = sqlite3_exec(DB, sql.c_str(), callback, 0, &errorMessage);

		sql = "UPDATE accounts SET balance = balance + " + to_string(amount) + " WHERE accountID = " + to_string(receiverAccountID) + ";";
		rc = sqlite3_exec(DB, sql.c_str(), callback, 0, &errorMessage);

		cout << "Transaction Completed." << endl;
		return true;
	}
	else
	{
		cout << "This account doesn't exist!" << endl;
		return false;
	}
}

/** @brief Fetches the customer information from the database
 *
 *	This method runs initially upon creation of the customer, and stores the user's password, name, credit score, loan debt, and usertype in
 *  the customer object's data members for later use.
 */
void customer::storeValues()
{
	// Declares the statement object
	sqlite3_stmt *stmt;

	// Retrieves the customer's password, name, credit score, loan debt, and user type
	sql = "SELECT password, name, creditScore, loanDebt, userType FROM users WHERE username = \"" + username + "\";";
	sqlite3_prepare_v2(DB, sql.c_str(), sql.length(), &stmt, nullptr);

	// Runs the statement
	step = sqlite3_step(stmt);

	// Parses the returned row, and stores each value in its respective data members
	password = string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
	name = string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)));
	creditScore = (sqlite3_column_int(stmt, 2));
	loanDebt = (sqlite3_column_double(stmt, 3));
	userType = string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4)));

	// Destroys the statement object
	sqlite3_finalize(stmt);
}

/** @brief Opens the database and fetches all existing accounts.
 *
 *  Takes in a username, opens the database, and stores all accounts under the user into the accounts vector. This method is used
 *  for GUI purposes
 *  @param username Represents the username of the regular user
 */
void customer::fill(string username)
{
	this->username = username;

	rc = sqlite3_open("newDatabase.db", &DB);

	vector<string> accountTypes;

	sqlite3_stmt *stmt;
	sql = "SELECT accountType FROM accounts WHERE username = \"" + username + "\";";
	sqlite3_prepare_v2(DB, sql.c_str(), sql.length(), &stmt, nullptr);
	int rc = sqlite3_step(stmt);
	while (rc != SQLITE_DONE)
	{
		string fetchedAccountType = string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
		cout << fetchedAccountType << endl;
		accountTypes.push_back(fetchedAccountType);
		rc = sqlite3_step(stmt);
	}

	cout << "Size of accountTypes is: " << accountTypes.size() << endl;

	if (accountTypes.size() > 0)
	{
		for (int i = 0; i < accountTypes.size(); i++)
		{
			account temp(accountTypes[i], username);
			accounts.push_back(temp);
		}
	}
	else
	{
		cout << "No Accounts" << endl;
	}

	sqlite3_finalize(stmt);

	storeValues();
}