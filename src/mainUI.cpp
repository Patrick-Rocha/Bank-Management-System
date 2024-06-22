/*
*	Filename: 		mainUI.cpp
*	Authors: 		Yazan Marwan Alazraq, Rami Istwani, Abdulrehman Khan, You-Chia Kuo, Patrick Rocha
*	Date: 			2022-11-08
*	Description: 	The main control of the program
*/

#include "login.h"

using namespace std;

/*
	Function: 		getType
	Description: 	returns the type of the user (customer or administrator)
	Parameters: 	N/A
*/ 
int main() {
    string username, password;      // Username and password that the user is about to enter
    bool loginVerification = false; // Verifies if the entered username and password are in the database
    login loginPage;                // Object to activate login.cpp's constructor

    // Asks the user to enter a username and password until their login is verified with loginVerification
    while (!loginVerification) {
        cout << "Enter username: ";
        getline(cin, username);
        cout << "Enter password: ";
        getline(cin, password);

        // Checks if the username and password are in the database
        if (loginPage.verifyLogin(username, password)) {
            cout << "Log in accepted" << endl;
            loginVerification = true;
            cout << "Account Type for " << username << " is: " << loginPage.checkUserType(username) << endl;
        } else {
            cout << "Username and password do not match. Please try again." << endl;
        }
    }
    return 1;
}