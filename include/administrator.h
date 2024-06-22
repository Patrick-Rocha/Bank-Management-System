/** @brief Provides the templace for administrator.
 * 
 *  Defines the variables and functions used by the administrator class.
 *  @authors Yazan Marwan Alazraq, Rami Istwani, Abdulrehman Khan, You-Chia Kuo, Patrick Rocha
 *  @file administrator.h
*/  

#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include <iostream>
#include "user.h"
#include "analytics.h"
#include "sqlite3.h"

class administrator : public user {
	private:
		sqlite3 *db;
		char *errorMessage;
		int rc, step, empty;
		std::string sql, user;
		bool userExists(std::string);
		bool accountExists(int);
	public:
		administrator();
		std::string getName(std::string);
		int getUserCreditScore(std::string);
		double getUserLoanDebt(std::string);
		std::string getUserType(std::string);
		void updateCreditScore(std::string, int);
		void removeUser(std::string);
		void giveLoan(int, double);
		void createUser(std::string, std::string, std::string);
};

#endif
