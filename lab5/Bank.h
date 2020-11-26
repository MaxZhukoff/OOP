#ifndef LAB_5_BANK_H
#define LAB_5_BANK_H

#include <iostream>
#include <vector>
#include "Time.h"
#include "Client.h"
#include "Account.h"



class Bank
{
public:
    Bank(const double creditCommission, const double limitationSum, const std::vector<std::pair<double, double>> &depositPercent)
    {
        this->creditCommission = creditCommission;
        this->limitationSum = limitationSum;
        for (int i = 0; i < depositPercent.size(); i++) {
            this->depositPercent[i] = depositPercent[i];
        }
    }

    



private:
    double creditCommission;
    double limitationSum;
    std::vector<std::pair<double, double>> depositPercent;
    std::vector<Client> *clients;
    std::vector<Account> *accounts;
};


#endif