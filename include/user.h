/** @brief Provides the templace for user.
 *
 *  Defines the variables and functions used by the user class
 *  @authors Yazan Marwan Alazraq, Rami Istwani, Abdulrehman Khan, You-Chia Kuo, Patrick Rocha
 *  @file user.h
 */

#ifndef USER_H
#define USER_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cstring>
#include <string>
#include <ctime>

class user
{
protected:
    std::string username;
    std::string password;
    std::string name;

public:
    std::string userType;
    std::string getPassword();
    std::string getUserType();
    std::string getName();
    std::string getUserName();
};

#endif