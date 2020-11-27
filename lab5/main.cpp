#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <optional>
#include "Bank.h"
#include "Account.h"
#include "Client.h"

using namespace std;

int main() {
    vector<pair<double, double>> depositPercent;
    depositPercent.push_back(make_pair(0, 5));
    depositPercent.push_back(make_pair(50000, 8));
    depositPercent.push_back(make_pair(100000, 10));
    Bank bank(10, 1, 100000, depositPercent);
    unsigned cl1 = bank.createClient().value();
    unsigned ac1 = bank.createDepositAccount(cl1, 10000, 12).value();
    unsigned ac2 = bank.createDebitAccount(cl1, 1000).value();
    bank.topUpMoney(ac2, 5000);
    cout << bank.showAccountMoney(ac2).value() << endl;
    bank.cancelingTransaction(0);
    cout << bank.showAccountMoney(ac2).value() << endl;
    bank.timeMachine(2022, 11, 27);
    bank.updateAccount(ac2);
    cout << bank.showAccountMoney(ac2).value() << endl;

//    t.timeMachine(2022, 12, 28);
//
//    deb1.checkPercent();
//    deb2.checkPercent();
//    cout << deb1.getMoney() << " " << deb2.getMoney() << endl;
//    deb1.transfer(&deb2, 5000);
//    cout << deb1.getMoney() << " " << deb2.getMoney() << endl;
//    deb1.topUpMoney(5000);
//    deb2.withdrawMoney(5000);
//    cout << deb1.getMoney() << " " << deb2.getMoney() << endl;

//    CreditAccount credit(&cl1, &t, 10, 1000);
//    cout << credit.getMoney() << endl;
//    credit.withdrawMoney(10000);
//    cout << credit.getMoney() << endl;
//    t.timeMachine(2025, 12, 27);
//    credit.checkCommission();
//    cout << credit.getMoney() << endl;

    return 0;
}