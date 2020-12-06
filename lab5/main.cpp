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
    unsigned cl2 = bank.createClient().value();
    cout << endl << endl;

    unsigned creditAc1 = bank.createCreditAccount(cl2, 5000).value();
    unsigned debitAc2 = bank.createDebitAccount(cl2, 10000).value();
    unsigned depositAc3 = bank.createDepositAccount(cl1, 50000, 6).value();
    cout << "Credit: " << bank.showAccountMoney(creditAc1).value() << endl;
    cout << "Debit: " << bank.showAccountMoney(debitAc2).value() << endl;
    cout << "Deposit: " << bank.showAccountMoney(depositAc3).value() << endl << endl;

    bank.withdrawMoney(debitAc2, 15000);
    bank.withdrawMoney(creditAc1, 9990);
    cout << "Credit: " << bank.showAccountMoney(creditAc1).value() << endl;
    cout << "Debit: " << bank.showAccountMoney(debitAc2).value() << endl << endl;

    unsigned tran1;
    auto tran = bank.withdrawMoney(debitAc2, 10000);
    cout << "Debit: " << bank.showAccountMoney(debitAc2).value() << endl;
    if (tran) {
        tran1 = tran.value();
        bank.cancelingTransaction(tran1);
        cout << "Debit cancel: " << bank.showAccountMoney(debitAc2).value() << endl << endl;
    }

    bank.transferMoney(debitAc2, creditAc1, 1000);
    cout << "Credit: " << bank.showAccountMoney(creditAc1).value() << endl;
    cout << "Debit: " << bank.showAccountMoney(debitAc2).value() << endl << endl;

    bank.timeMachine(2021, 06, 27);
    cout << "Credit: " << bank.showAccountMoney(creditAc1).value() << endl;
    cout << "Debit: " << bank.showAccountMoney(debitAc2).value() << endl;
    cout << "Deposit: " << bank.showAccountMoney(depositAc3).value() << endl << endl;


    return 0;
}