#ifndef LAB_5_TIME_H
#define LAB_5_TIME_H

#include <iostream>
#include <ctime>
#include "Account.h"

class GlobalTime {
public:
    GlobalTime()
    {
        nowSec = time(0);
        ltm = localtime(&nowSec);
    }
    void upDate()
    {
        nowSec = time(0);
        ltm = localtime(&nowSec);
    }
    void timeMachine(const int year, const int month, const int day)
    {
        ltm->tm_year = year - 1900;
        ltm->tm_mon = month - 1;
        ltm->tm_mday = day;
        nowSec = mktime(ltm);
    }
    int getYear() const { return ltm->tm_year + 1900; }
    int getMonth() const { return  ltm->tm_mon + 1; }
    int getDay() const { return ltm->tm_mday; }
    int getSecFrom() const { return nowSec; }

private:
    time_t nowSec;
    tm *ltm;
};

class AccountTime {
public:
    AccountTime(GlobalTime *time)
    {
        globalTime = time;
        dayStart = time->getDay();
        monthStart = time->getMonth();
        yearStart = time->getYear();
        secNow = globalTime->getSecFrom();
    }

    virtual std::optional<std::pair<int, int>> checkDate()
    {
        int dayNow = globalTime->getDay();
        int monthNow = globalTime->getMonth();
        int yearNow = globalTime->getYear();
        time_t sec = globalTime->getSecFrom();
        time_t diffSec = difftime(sec, secNow);
        if (diffSec <= 0)
            return {};
        tm *ltmDiff = localtime(&diffSec);
        int yearsDiff = ltmDiff->tm_year - 70;
        int monthsDiff = yearsDiff * 12 + ltmDiff->tm_mon;
        int daysDiff = yearsDiff * 365 + ltmDiff->tm_yday;
        secNow = globalTime->getSecFrom();
        if (monthsDiff == 0 && dayNow >= dayStart && (monthNow > monthStart || yearNow > yearStart))
            monthsDiff++;
        return std::make_pair(monthsDiff, daysDiff);
    }

protected:
    GlobalTime *globalTime;
    int yearStart;
    int monthStart;
    int dayStart;
    time_t secNow;
};

class DepositAccountTime : public AccountTime {
public:
    DepositAccountTime(GlobalTime *time, const int months) : AccountTime(time)
    {
        if (months > 0)
            activated = false;
        globalTime = time;
        dayStart = time->getDay();
        monthStart = time->getMonth();
        yearStart = time->getYear();
        secNow = globalTime->getSecFrom();
        ltmEnd = localtime(&secNow);
        ltmEnd->tm_mon += months;
        secEnd = mktime(ltmEnd);
    }

    std::optional<std::pair<int, int>> checkDate() override
    {
        if (activated)
            return {};
        int dayNow = globalTime->getDay();
        int monthNow = globalTime->getMonth();
        int yearNow = globalTime->getYear();
        time_t sec;
        if (globalTime->getSecFrom() > secEnd)
        {
            sec = secEnd;
            activated = true;
        }
        else
            sec = globalTime->getSecFrom();
        time_t diffSec = difftime(sec, secNow);
        if (diffSec <= 0)
            return {};
        tm *ltmDiff = localtime(&diffSec);
        int yearsDiff = ltmDiff->tm_year - 70;
        int monthsDiff = yearsDiff * 12 + ltmDiff->tm_mon;
        int daysDiff = yearsDiff * 365 + ltmDiff->tm_yday;
        secNow = globalTime->getSecFrom();
        if (monthsDiff == 0 && dayNow >= dayStart && (monthNow > monthStart || yearNow > yearStart))
            monthsDiff++;
        return std::make_pair(monthsDiff, daysDiff);
    }

    bool checkActivated()const { return activated; }

private:
    bool activated;
    time_t secEnd;
    tm *ltmEnd;
};

#endif