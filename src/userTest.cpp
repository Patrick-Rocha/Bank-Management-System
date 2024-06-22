#include "customer.h"
#include "account.h"

using namespace std;

int main() {
    customer person1;

    cout << "\nCreating Chequing Account." << endl;

    person1.createAccount("Chequing Account", 500.00);

    cout << "Chequing Account Balance: " << person1.checkAccountBalance("Chequing Account") << endl;

    cout << "\nCreating Savings Account." << endl;

    person1.createAccount("Savings Account", 0);

    cout << "Savings Account Balance: " << person1.checkAccountBalance("Savings Account") << endl;

    person1.transaction("Chequing Account", "Savings Account", 250.00);

    cout << "\nTransferring $250.00 from Chequing Account to Savings Account." << endl;
    cout << "Chequing Account Balance: " << person1.checkAccountBalance("Chequing Account") << endl;
    cout << "Savings Account Balance: " << person1.checkAccountBalance("Savings Account") << endl;

    cout << "\nWithdrawing $50 from Chequing Account" << endl;
    person1.withdraw("Chequing Account", 50.00);
    cout << "Chequing Account Balance: " << person1.checkAccountBalance("Chequing Account") << endl;

    cout << "\nDepositing $23 from Savings Account" << endl;
    person1.deposit("Savings Account", 23.00);
    cout << "Savings Account Balance: " << person1.checkAccountBalance("Savings Account") << endl;
}