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
    Account(Client *client, const double money = 0)
    {
        if (money < 0)
            this->money = 0;
        else
            this->money = money;
        this->client = client;
        activated = true;
    }

    virtual bool withdrawMoney(const double money)
    {
        if (!activated)
            return false;
        if (this->money - money < 0 || money < 0)
        {
            std::cerr << "There is not enough money in the account" << std::endl;
            return false;
        }
        this->money -= money;
        return true;
    }

    void bankWithdrawMoney(const double money) { this->money -= money; }

    virtual bool transfer(Account *otherAccount, const double money)
    {
        if (!activated)
            return false;
        if (this->money - money < 0 || money < 0) {
            std::cerr << "There is not enough money in the account" << std::endl;
            return false;
        }
        this->money -= money;
        otherAccount->topUpMoney(money);
        return true;
    }

    virtual bool topUpMoney(const double money)
    {
        if (!activated || money < 0)
            return false;
        this->money += money;
        return true;
    }

    double getMoney()const { return money; }

protected:
    Client *client;
    bool activated;
    unsigned clientID;
    double money;
};

class DebitAccount : public Account
{
public:
    DebitAccount(Client *client, GlobalTime *time, const double percent, const double money = 0) : Account(client, money), accountTime(time)
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

    void checkPercent()
    {
        auto res = accountTime.checkDate();
        if (res.operator bool())
        {
            std::pair<int, int> months_days = res.value();
            accrual(months_days);
        }
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
    Deposit(Client *client, GlobalTime *time, const double initialDeposit, const double percent, const int months) : Account(client), accountTime(time, months)
    {
        if (initialDeposit < 0)
            deposit = 0;
        else
            deposit = initialDeposit;
        activated = false;
        accruals = 0;
        if (percent <= 0)
            initialPercent = 0.1;
        else
            initialPercent = percent;
        sumPercents = initialDeposit * (percent / 360 / 100);
    }

    bool checkDeposit()
    {
        if (!activated)
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
                activated = true;
                return true;
            }
            else return false;
        }
        else return true;
    }

    bool topUpMoney(const double money) override
    {
        if (money < 0)
            return false;
        if (!activated)
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
    double sumPercents;
    double initialPercent;
    double deposit;
    double accruals;
};

class CreditAccount : public Account
{
public:
    CreditAccount(Client *client, GlobalTime *time, const double commission, const double money = 0) : Account(client, money), accountTime(time)
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
        this->money -= money;
        if (checkCommission())
            this->money -= commission;
        return true;
    }

    bool transfer(Account *otherAccount, const double money) override
    {
        if (!activated)
            return false;
        this->money -= money;
        otherAccount->topUpMoney(money);
        if (checkCommission())
            this->money -= commission;
        return true;
    }

    bool topUpMoney(const double money) override
    {
        if (!activated || money < 0)
            return false;
        this->money += money;
        checkCommission();
        return true;
    }

    bool checkCommission() {
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