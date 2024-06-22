#include "administrator.h"
using namespace std;

int main() {
    administrator admin;

    cout << admin.getUserType("admin001") << endl;
/*
    cout << "CREDIT = " << admin.getUserCreditScore("user001") << endl;
    cout << "USERNAME = " << admin.getUsername("user005") << endl;
    cout << "LOANDEBT USER2 = " << admin.getUserLoanDebt("user002") << endl;
    admin.createUser("AYAYA", "user002", "twouser");
    admin.removeUser("user006");
    admin.updateCreditScore("user006", 800);
    admin.updateCreditScore("user004", 40);
    admin.giveLoan(1, 501400.78);*/
    //admin.createUser("BOBBY", "user003", "threeuser");
    return 1;
}