/** @brief Provides the templace for customer.
 *
 *  Defines the variables and functions used by the customer class
 *  @authors Yazan Marwan Alazraq, Rami Istwani, Abdulrehman Khan, You-Chia Kuo, Patrick Rocha
 *  @file customer.h
 */

#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "sqlite3.h"
#include "user.h"
#include "account.h"

class customer : public user
{
private:
    sqlite3 *DB;
    std::string sql;
    int rc, step;
    int creditScore;
    double loanDebt;
    double money;

public:
    std::vector<account> accounts;
    customer(std::string username);
    ~customer();
    int getCreditScore();
    double getMoney();
    double getLoanDebt();
    double checkAccountBalance(std::string accountType);
    bool createAccount(std::string accountType, double smoney);
    bool deleteAccount(std::string accountType);
    bool transaction(int accountID, int receiverAccountID, double amount);
    void storeValues();
    void fill(std::string username);
};

#endif