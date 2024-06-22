/** @brief Provides the templace for analytics.
 * 
 *  Defines the variables and functions used by the analytics class.
 *  @authors Yazan Marwan Alazraq, Rami Istwani, Abdulrehman Khan, You-Chia Kuo, Patrick Rocha
 *  @file analytics.h
*/  

#ifndef ANALYTICS_H
#define ANALYTICS_H

#include <iostream>
#include <iomanip>
#include <math.h>
#include <string>
#include "sqlite3.h"

class analytics {
    private:
    	sqlite3 *db;
        std::string sql;
        char *errorMessage;
        int totalTransactions, rc, step, empty;
        double averageCredit, averageBalance;
        void calculateAverageBalance();
        void calculateAverageCreditScore();
        bool userExists(std::string);
    public:
        analytics();
        int getNumUsers();
        double getBalance(std::string);
        double getAverageBalance();
        int getNumTransactions();
        double getAverageCreditScore();     
        int getCreditScore(std::string);
};

#endif