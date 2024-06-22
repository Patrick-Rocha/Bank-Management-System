/** @brief Provides the templace for budgeting.
 *
 *  Defines the variables and functions used by the budgeting class
 *  @authors Yazan Marwan Alazraq, Rami Istwani, Abdulrehman Khan, You-Chia Kuo, Patrick Rocha
 *  @file budgeting.h
 */

#ifndef BUDGET_CLASS_H
#define BUDGET_CLASS_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "sqlite3.h"

class budgeting
{
private:
    sqlite3 *DB;
    char *errorMessage;
    std::string sql;
    std::string username;
    double spending;
    double moneyGained;
    double initialBalance;
    int rc, step;

public:
    budgeting(std::string username);
    budgeting();
    ~budgeting();
    double getSpending();
    double getGained();
    double getProfit();
    double getInitialBalance();
};

#endif