#ifndef LAB_5_COMMAND_H
#define LAB_5_COMMAND_H

#include <optional>
#include "Account.h"

class Command {
public:
    Command(const double money, const unsigned ID)
    {
        flag = false;
        this->money = money;
        this->ID = ID;
    }
    virtual bool execute() { return false; }
    virtual bool undo() { return false; }
    unsigned getID()const { return ID; }

protected:
    bool flag;
    unsigned ID;
    double money;
};

class WithdrawMoneyCommand : public Command {
public:
    WithdrawMoneyCommand(Account *account, const double money, const unsigned ID) : Command(money, ID) { this->account = account; }
    bool execute() override
    {
        if (money < 0 || flag)
            return false;
        if (account->withdrawMoney(money))
        {
            flag = true;
            return true;
        }
        return false;
    }

    bool undo() override
    {
        if (money < 0 || !flag)
            return false;
        account->topUpMoney(money);
        flag = false;
        return true;
    }

private:
    Account *account;
};

class TransferMoneyCommand : public Command {
public:
    TransferMoneyCommand(Account *from, Account *in, const double money, const unsigned ID) : Command(money, ID)
    {
        this->from = from;
        this->in = in;
    }

    bool execute() override
    {
        if (money < 0 || flag)
            return false;
        if (from->transferMoney(in, money))
        {
            flag = true;
            return true;
        }
        return false;
    }

    bool undo() override
    {
        if (money < 0 || !flag)
            return false;
        from->topUpMoney(money);
        in->bankWithdrawMoney(money);
        flag = false;
        return true;
    }

private:
    Account *from, *in;
};

class TopUpMoneyCommand : public Command {
public:
    TopUpMoneyCommand(Account *account, const double money, const unsigned ID) : Command(money, ID) { this->account = account; }
    bool execute() override
    {
        if (money < 0 || flag)
            return false;
        if (account->topUpMoney(money))
        {
            flag = true;
            return true;
        }
        return false;
    }

    bool undo() override
    {
        if (money < 0 || !flag)
            return false;
        account->bankWithdrawMoney(money);
        flag = false;
        return true;
    }

private:
    Account *account;
};

#endif