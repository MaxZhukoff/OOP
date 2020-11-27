#ifndef LAB_5_BANK_H
#define LAB_5_BANK_H

#include <iostream>
#include <vector>
#include "Time.h"
#include "Client.h"
#include "Account.h"

enum transactionType {
    withdraw,
    transfer,
    topUp
};

class Bank
{
public:
    Bank(const double creditCommission, const double debitPercent, const double limitationSum, const std::vector<std::pair<double, double>> &depositPercents)
    {
        this->creditCommission = creditCommission;
        this->debitPercent = debitPercent;
        this->limitationSum = limitationSum;
        for (const auto & i : depositPercents)
            this->depositPercents.push_back(i);
    }

    std::optional<unsigned> createClient()
    {
        clientBuilder.reset();
        std::string firstName, lastName, address;
        unsigned pasNum;
        char answer;
        std::cout << "Enter first and last name:\n";
//        std::cin >> firstName, lastName;
        firstName = "Alexey";//
        lastName = "Gybanov";//
        clientBuilder.setName(firstName, lastName);
        std::cout << "Do you want to enter an address?(y/n):\n";
//        std::cin >> answer;
        answer = 'y';//
        if (answer == 'y')
        {
            std::cout << "Enter the address:\n";
//            std::cin >> address;
            address = "Tverskaya 5";//
            clientBuilder.setAddress(address);
        }
        std::cout << "Do you want to enter a passport number?(y/n):\n";
//        std::cin >> answer;
        if (answer == 'y')
        {
            std::cout << "Enter passport number:\n";
//            std::cin >> pasNum;
            pasNum = 133745;
            clientBuilder.setPass(pasNum);
        }
        auto res = clientBuilder.build(clients.size());
        if (res.operator bool())
        {
            Client *client = new Client(res.value());
            clients.push_back(client);
            return client->getID();
        }
        return {};
    }

    void addClientInfo(const unsigned clientID)
    {
        std::string address;
        int pasNum;
        char answer = 'y';//
        if (clients[clientID]->getAddress().empty())
        {
            std::cout << "Do you want to enter an address?(y/n):\n";
//        std::cin >> answer;
            answer = 'y';//
            if (answer == 'y')
            {
                std::cout << "Enter the address:\n";
//            std::cin >> address;
                address = "Tverskaya 5";//
                clients[clientID]->addAddress(address);
            }
        }
        if (clients[clientID]->getPassNum() == 0)
        {
            std::cout << "Do you want to enter a passport number?(y/n):\n";
//        std::cin >> answer;
            if (answer == 'y')
            {
                std::cout << "Enter passport number:\n";
//            std::cin >> pasNum;
                pasNum = 133745;
                clients[clientID]->addPassNum(pasNum);
            }
        }
    }

    std::optional<unsigned> createDebitAccount(const unsigned ClientID, const double money)
    {
        if (money < 0 || ClientID > clients.size() - 1)
            return {};
        DebitAccount *debitAccount = new DebitAccount(clients[ClientID], &time, accounts.size(), debitPercent, limitationSum, money);
        accounts.push_back(debitAccount);
        return debitAccount->getID();
    }

    std::optional<unsigned> createDepositAccount(const unsigned ClientID, const double initialDeposit, const unsigned months)
    {
        if (initialDeposit < 0 || months < 0 || ClientID > clients.size() - 1)
            return {};
        double depositPercent = 0;
        int index = 0;
        if (depositPercents.size() > 1)
        {
            while (initialDeposit >= depositPercents[index].first)
                if (index + 1 <= depositPercents.size() - 1) {
                    index++;
                    if (initialDeposit < depositPercents[index].first) {
                        depositPercent = depositPercents[index - 1].second;
                        break;
                    }
                }
                else {
                    depositPercent = depositPercents[index].second;
                    break;
                }
            if (depositPercent == 0)
                depositPercent = depositPercents[index].second;
        }
        else if (depositPercents.size() == 1)
            depositPercent = depositPercents[index].second;
        if (depositPercent == 0)
            return {};
        Deposit *deposit = new Deposit(clients[ClientID], &time, accounts.size(), initialDeposit, depositPercent, months, limitationSum);
        accounts.push_back(deposit);
        return deposit->getID();
    }

    bool createCreditAccount(const unsigned ClientID, const double money)
    {
        if (money < 0 || ClientID > clients.size() - 1)
            return {};
        CreditAccount *creditAccount = new CreditAccount(clients[ClientID], &time, accounts.size(), creditCommission, limitationSum, money);
        accounts.push_back(creditAccount);
        return creditAccount->getID();
    }

    std::optional<unsigned> withdrawMoney(unsigned accountID, const double money)
    {
        if (money < 0 || accountID > accounts.size() - 1)
            return {};
        unsigned transactionID = transactions.size();
        if (accounts[accountID]->withdrawMoney(money))
        {
            std::pair account(accounts[accountID], accounts[accountID]);
            std::pair type(withdraw, money);
            std::pair transaction(type, account);
            return transactionID;
        }
        else return {};
    }

    std::optional<unsigned> transferMoney(unsigned accountID, unsigned otherAccountID, const double money)
    {
        if (money < 0 || accountID > accounts.size() - 1)
            return {};
        unsigned transactionID = transactions.size();
        if (accounts[accountID]->transferMoney(accounts[otherAccountID], money))
        {
            std::pair account(accounts[accountID], accounts[otherAccountID]);
            std::pair type(transfer, money);
            std::pair transaction(type, account);
            transactions.push_back(transaction);
            return transactionID;
        }
        else return {};
    }

    std::optional<unsigned> topUpMoney(unsigned accountID, const double money)
    {
        if (money < 0 || accountID > accounts.size() - 1)
            return {};
        unsigned transactionID = transactions.size();
        if (accounts[accountID]->topUpMoney(money))
        {
            std::pair account(accounts[accountID], accounts[accountID]);
            std::pair type(topUp, money);
            std::pair transaction(type, account);
            transactions.push_back(transaction);
            return transactionID;
        }
        else return {};
    }

    void updateAccount(const unsigned accountId) { accounts[accountId]->updateAccount(); }

    std::optional<double> showAccountMoney(unsigned accountID)
    {
        if (accountID > accounts.size() - 1)
            return {};
        return accounts[accountID]->getMoney();
    }

    bool cancelingTransaction(unsigned transactionID)
    {
        if (transactionID > transactions.size() - 1)
            return false;
        if (transactions[transactionID].first.first == withdraw)
        {
            transactions[transactionID].second.first->topUpMoney(transactions[transactionID].first.second);
            transactions[transactionID].first.second = 0;
            return true;
        }
        if (transactions[transactionID].first.first == transfer)
        {
            transactions[transactionID].second.first->topUpMoney(transactions[transactionID].first.second);
            transactions[transactionID].second.second->bankWithdrawMoney(transactions[transactionID].first.second);
            transactions[transactionID].first.second = 0;
            return true;
        }
        if (transactions[transactionID].first.first == topUp)
        {
            transactions[transactionID].second.first->bankWithdrawMoney(transactions[transactionID].first.second);
            transactions[transactionID].first.second = 0;
            return true;
        }
        return false;
    }

    void timeMachine(const int year, const int month, const int day) { time.timeMachine(year, month, day); }
    void timeUpdate() { time.upDate(); }

    ~Bank()
    {
        for (const auto & i : clients) {
            delete i;

        }
        for (const auto & i : accounts) {
            delete i;
        }
    }

private:
    GlobalTime time;
    ClientBuilder clientBuilder;
    double debitPercent;
    double creditCommission;
    double limitationSum;
    std::vector<std::pair<double, double>> depositPercents;
    std::vector<Client*> clients;
    std::vector<Account*> accounts;
    std::vector<std::pair<std::pair<transactionType, double>, std::pair<Account*, Account*>>> transactions;
};

#endif