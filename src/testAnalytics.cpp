#include "analytics.h"
using namespace std;

int main() {
    analytics analyticsPage;

    cout << "Num users = " << analyticsPage.getNumUsers() << endl;
    cout << "balance user1 = " << fixed << setprecision(2) << showpoint << analyticsPage.getBalance("user001") << endl;
    cout << "balance user2 = " << analyticsPage.getBalance("user002") << endl;
    cout << "balance user3 = " << analyticsPage.getBalance("user003") << endl;
    cout << "balance user4 = " << analyticsPage.getBalance("user004") << endl;
    cout << "balance NONEXISTANT = " << analyticsPage.getBalance("ua89sdhas") << endl;
    cout << "average bal = " << analyticsPage.getAverageBalance() << endl;
    cout << "numTransactions = " << analyticsPage.getNumTransactions() << endl;
    cout << "average credit = " << analyticsPage.getAverageCreditScore() << endl;
    cout << "credit score user1 = " << analyticsPage.getCreditScore("user001") << endl;
    cout << "credit score NONEXISTANT = " << analyticsPage.getCreditScore("user01232131201") << endl;
    return 1;
}