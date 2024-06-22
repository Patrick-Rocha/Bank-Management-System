/** @brief abstract interface representing a user
 *
 *  Abstract interface for both customer-type users and administrator-type users
 *  @authors Yazan Marwan Alazraq, Rami Istwani, Abdulrehman Khan, You-Chia Kuo, Patrick Rocha
 *  @file user.cpp
 *  @class user "../include/user.h"
 */

#include "user.h"

using namespace std;

/** @brief returns the type of the user (regular or administrator
 *
 *  This method returns the type of user
 *  @return returns the user type
 */
string user::getUserType()
{
	return userType;
}

/** @brief returns the password of the user
 *
 *  This method returns the password that the user uses to login
 *  @return returns the user password
 */
string user::getPassword()
{
	return password;
}

/** @brief returns the username of the user
 *
 *  This method returns the name of the user
 *  @return returns the name of the user
 */
string user::getName()
{
	return name;
}

/** @brief returns the username of the user
 *
 *  This method returns the username that the user uses to login
 *  @return returns the username
 */
string user::getUserName()
{
	return username;
}
