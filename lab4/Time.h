#ifndef LAB_4_TIME_H
#define LAB_4_TIME_H

#include <iostream>
#include <optional>
#include <ctime>

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
    void timeMachine(const int sec)
    {
        time_t t = sec;
        nowSec = time(&t);
        ltm = localtime(&nowSec);
    }
    int getYear() const { return ltm->tm_year + 1900; }
    int getMonth() const { return  ltm->tm_mon + 1; }
    int getDay() const { return ltm->tm_mday; }
    int getSecFrom() const { return nowSec; }

private:
    time_t nowSec;
    tm *ltm;
};

class CreateTime {
public:
    CreateTime(GlobalTime *time)
    {
        globalTime = time;
        creationDay = time->getDay();
        creationMonth = time->getMonth();
        creationYear = time->getYear();
        creationSec = globalTime->getSecFrom();
    }

    int getCreationYear() const { return creationYear + 1900; }
    int getCreationMonth() const { return  creationMonth + 1; }
    int getCreationDay() const { return creationDay; }
    int getCreationSecFrom() const { return creationSec; }

private:
    GlobalTime *globalTime;
    int creationYear;
    int creationMonth;
    int creationDay;
    time_t creationSec;
};

#endif