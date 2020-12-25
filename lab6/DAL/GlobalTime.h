#ifndef LAB6_GLOBALTIME_H
#define LAB6_GLOBALTIME_H

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

    bool operator<(const GlobalTime& other) const
    {
        return this->getSecFrom() < other.getSecFrom();
    }

    int getYear() const { return ltm->tm_year + 1900; }
    int getMonth() const { return  ltm->tm_mon + 1; }
    int getDay() const { return ltm->tm_mday; }
    int getSecFrom() const { return nowSec; }

private:
    time_t nowSec;
    tm *ltm;
};

class Time {
public:
    Time(GlobalTime *time)
    {
        day = time->getDay();
        month = time->getMonth();
        year = time->getYear();
        sec = time->getSecFrom();
    }
    int getYear() const { return year; }
    int getMonth() const { return  month; }
    int getDay() const { return day; }
    int getSecFrom() const { return sec; }

private:
    int year;
    int month;
    int day;
    time_t sec;
};

#endif