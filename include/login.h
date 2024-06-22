/** @brief Provides the templace for login.
 * 
 *  Defines the variables and functions used by the login class.
 *  @authors Yazan Marwan Alazraq, Rami Istwani, Abdulrehman Khan, You-Chia Kuo, Patrick Rocha
 *  @file login.h
*/  

#ifndef LOGIN_H
#define LOGIN_H

#include <iostream>
#include <stdio.h>
#include "sqlite3.h"

class login {
	private:
		sqlite3 *db;
		int rc, step;
		std::string sql;
		char *errorMessage;
		bool accountFound;`
	public:
		login();
		bool verifyLogin(std::string, std::string);
		std::string checkUserType(std::string);
};

#endif
