#ifndef LAB_5_ACCOUNT_H
#define LAB_5_ACCOUNT_H

#include <iostream>
#include <cmath>
#include <ctime>
#include "Time.h"
#include "Client.h"

class Account
{
public:
    Account(Client *client, const unsigned ID, const double limitationSum, const double money = 0)
    {
        if (money < 0)
            this->money = 0;
        else
            this->money = money;
        this->ID = ID;
        clientID = client->getID();
        this->client = client;
        activated = true;
        this->limitationSum = limitationSum;
    }

    virtual bool withdrawMoney(const double money)
    {
        updateAccount();
        if (!activated)
            return false;
        if (this->money - money < 0 || money < 0)
        {
            std::cerr << "There is not enough money in the account" << std::endl;
            return false;
        }
        if (!client->getTrusted() && money > limitationSum)
        {
            std::cerr << "You cannot transfer more than " << limitationSum << std::endl;
            return false;
        }
        this->money -= money;
        return true;
    }

    void bankWithdrawMoney(const double money) { this->money -= money; }

    virtual bool transferMoney(Account *otherAccount, const double money)
    {
        updateAccount();
        if (!activated)
            return false;
        if (this->money - money < 0 || money < 0) {
            std::cerr << "There is not enough money in the account" << std::endl;
            return false;
        }
        if (!client->getTrusted() && money > limitationSum)
        {
            std::cerr << "You cannot transfer more than " << limitationSum << std::endl;
            return false;
        }
        this->money -= money;
        otherAccount->topUpMoney(money);
        return true;
    }

    virtual bool topUpMoney(const double money)
    {
        updateAccount();
        if (!activated || money < 0)
            return false;
        if (!client->getTrusted() && money > limitationSum)
        {
            std::cerr << "You cannot transfer more than " << limitationSum << std::endl;
            return false;
        }
        this->money += money;
        return true;
    }

    virtual bool updateAccount() { return true; }

    double getMoney()const { return money; }
    unsigned getID()const { return ID; }
    unsigned getClientID()const { return clientID; }

protected:
    Client *client;
    bool activated;
    unsigned ID;
    unsigned clientID;
    double money;
    double limitationSum;
};

class DebitAccount : public Account
{
public:
    DebitAccount(Client *client, GlobalTime *time, const unsigned ID, const double percent, const double limitationSum, const double money = 0) : Account(client, ID, limitationSum, money), accountTime(time)
    {
        if (money < 0)
            this->money = 0;
        else
            this->money = money;
        accruals = 0;
        if (percent <= 0)
            initialPercent = 0.1;
        else
            initialPercent = percent;
        sumPercents = money * (percent / 360 / 100);
    }

    bool updateAccount() override
    {
        auto res = accountTime.checkDate();
        if (res.operator bool())
        {
            std::pair<int, int> months_days = res.value();
            accrual(months_days);
        }
        return true;
    }

private:
    void accrual(const std::pair<int, int> months_days) {
        int months = months_days.first;
        int days = months_days.second;
        if (months < 1)
            for (days; days > 0; days--)
                accruals += sumPercents;
        for (months; months > 0; months--) {
            for (int i = days / months; i > 0; i--)
                accruals += sumPercents;
            money += accruals;
            sumPercents = money * (initialPercent / 360 / 100);
        }
    }

    AccountTime accountTime;
    double sumPercents;
    double initialPercent;
    double accruals;
};

class Deposit : public Account
{
public:
    Deposit(Client *client, GlobalTime *time, const unsigned ID, const double initialDeposit, const double percent, const int months, const double limitationSum) : Account(client, ID, limitationSum), accountTime(time, months)
    {
        if (initialDeposit < 0)
            deposit = 0;
        else
            deposit = initialDeposit;
        depositActivated = false;
        accruals = 0;
        if (percent <= 0)
            initialPercent = 0.1;
        else
            initialPercent = percent;
        sumPercents = initialDeposit * (percent / 360 / 100);
    }

    bool updateAccount() override
    {
        if (!depositActivated)
        {
            auto res = accountTime.checkDate();
            if (res.operator bool())
            {
                std::pair<int, int> months_days = res.value();
                accrual(months_days);
            }
            if (accountTime.checkActivated())
            {
                money = deposit;
                deposit = 0;
                depositActivated = true;
                return true;
            }
            else return false;
        }
        else return true;
    }

    bool topUpMoney(const double money) override
    {
        updateAccount();
        if (money < 0)
            return false;
        if (!client->getTrusted() && money > limitationSum)
        {
            std::cerr << "You cannot transfer more than " << limitationSum << std::endl;
            return false;
        }
        if (!depositActivated)
            deposit += money;
        else
            this->money += money;
        return true;
    }

private:
    void accrual(const std::pair<int, int> months_days) {
        int months = months_days.first;
        int days = months_days.second;
        if (months < 1)
            for (days; days > 0; days--)
                accruals += sumPercents;
        for (months; months > 0; months--) {
            for (int i = days / months; i > 0; i--)
                accruals += sumPercents;
            deposit += accruals;
            sumPercents = deposit * (initialPercent / 360 / 100);
        }
    }

    DepositAccountTime accountTime;
    bool depositActivated;
    double sumPercents;
    double initialPercent;
    double deposit;
    double accruals;
};

class CreditAccount : public Account
{
public:
    CreditAccount(Client *client, GlobalTime *time, const unsigned ID, const double commission, const double limitationSum, const double money = 0) : Account(client, ID, limitationSum, money), accountTime(time)
    {
        this->commission = commission;
        if (money < 0)
            this->money = 0;
        boolCommission = false;
        sumCommission = 0;
    }

    bool withdrawMoney(const double money) override
    {
        if (!activated)
            return false;
        if (!client->getTrusted() && money > limitationSum)
        {
            std::cerr << "You cannot transfer more than " << limitationSum << std::endl;
            return false;
        }
        this->money -= money;
        if (updateAccount())
            this->money -= commission;
        return true;
    }

    bool transferMoney(Account *otherAccount, const double money) override
    {
        if (!activated)
            return false;
        if (!client->getTrusted() && money > limitationSum)
        {
            std::cerr << "You cannot transfer more than " << limitationSum << std::endl;
            return false;
        }
        this->money -= money;
        otherAccount->topUpMoney(money);
        if (updateAccount())
            this->money -= commission;
        return true;
    }

    bool topUpMoney(const double money) override
    {
        if (!activated || money < 0)
            return false;
        if (!client->getTrusted() && money > limitationSum)
        {
            std::cerr << "You cannot transfer more than " << limitationSum << std::endl;
            return false;
        }
        this->money += money;
        updateAccount();
        return true;
    }

    bool updateAccount() {
        if (money < 0) {
            auto res = accountTime.checkDate(boolCommission);
            if (res.operator bool())
            {
                std::pair<int, int> months_days = res.value();
                accrual(months_days);
            }
            boolCommission = true;
            return true;
        }
        else return false;
    }

private:
    void accrual(const std::pair<int, int> months_days) {
        int months = months_days.first;
        int days = months_days.second;
        if (days == 0 && months == 1)
        {
            money -= sumCommission;
            sumCommission = 0;
        }
        if (months < 1)
            sumCommission += commission * days;
        else {
            sumCommission += commission * days;
            money -= sumCommission;
            sumCommission = 0;
        }
    }

    CreditAccountTime accountTime;
    double commission;
    bool boolCommission;
    double sumCommission;
};

#endif