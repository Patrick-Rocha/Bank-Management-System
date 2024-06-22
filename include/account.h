/** @brief Provides the templace for account
 *  @authors Yazan Marwan Alazraq, Rami Istwani, Abdulrehman Khan, You-Chia Kuo, Patrick Rocha
 *  @file account.h
 *
 *  Defines the variables and functions used by the account class
 */

#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <cmath>
#include "sqlite3.h"

class account
{
private:
    sqlite3 *DB;
    char *errorMessage;
    std::string sql;
    std::string username;
    std::string accountType;
    double balance;
    int accountID;
    int rc, step;

public:
    account(std::string accountType, std::string username, double initialMoney); // For creating an account
    account(std::string accountType, std::string username);                      // For storing existing accounts
    ~account();
    double getBalance(); // Returns balance for this account
    bool applyForLoan(double amount);
    int getID();                  // Returns accountID for this account
    std::string getUserName();    // Returns the username the account belongs to
    std::string getAccountType(); // Returns the accountType for this account
    bool withdraw(double amount); // Withdraws money from the account
    bool deposit(double amount);  // Deposits money into the account
    void storeValues();           // Resets values for accountID, balance, and accountType
    void refreshBalance();        // Refreshes value for balance
};

#endif