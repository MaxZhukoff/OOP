#ifndef LAB6_TASK_H
#define LAB6_TASK_H

#include <string>
#include <vector>
#include <set>
#include "GlobalTime.h"
#include "Staff.h"

enum TaskStatus {
    Open,
    Active,
    Resolved
};

class Log {
public:
    Log(GlobalTime *time, Staff *staff, const std::string &log) : timeLog(time)
    {
        this->log = log;
        this->staff = staff;
    }

    Staff* getStaff() { return staff; }
    Time* getTime() { return &timeLog; }

private:
    std::string log;
    Staff *staff;
    Time timeLog;
};

class TaskLogger {
public:
    TaskLogger(GlobalTime *time)
    {
        this->time = time;
    }

    void add(const std::string &log, Staff *staff)
    {
        Log *log1 = new Log(time, staff, log);
        time->upDate();
        logs.push_back(log1);
        changer.emplace(staff);
    }

    std::optional<Staff*> findStaff(Staff *staff)
    {
        auto it = changer.find(staff);
        if (it != changer.end())
            return *it;
        else return { };
    }

    std::vector<Log*> getAll()const { return logs; }

private:
    GlobalTime *time;
    std::vector<Log*> logs;
    std::set<Staff*> changer;
};

class Task {
public:
    Task(const std::string name, const std::string description, Staff *staff, GlobalTime *time, const unsigned id) : taskTime(time), taskLogger(time) {
        this->name = name;
        this->description = description;
        this->id = id;
        status = Open;
        this->staff = staff;
        std::string log = "Employee appointed: ";
        log += staff->getName();
        taskLogger.add(log, staff);
    }

    void changeStatus(TaskStatus status, Staff *staff)
    {
        this->status = status;
        std::string log = "Change status: ";
        if (status == Open) log += "Open";
        else if (status == Active) log += "Active";
        else if (status == Resolved) log += "Resolved";
        taskLogger.add(log, staff);
    }

    void addComment(const std::string &comment, Staff *staff)
    {
        comments.push_back(comment);
        std::string log = "Add comment: ";
        log += comment;
        taskLogger.add(log, staff);
    }

    void changeStaff(Staff *staff)
    {
        this->staff = staff;
        std::string log = "Change staff: ";
        log += staff->getName();
        taskLogger.add(log, staff);
    }

    unsigned getId()const { return id; }

    std::string getName()const { return name; }

    Staff* getStaff()const { return staff; }

    TaskLogger* getLogger() { return &taskLogger; }

    TaskStatus getStatus()const { return status; }

    bool operator<(const Task& other) const { return this->getId() < other.getId(); }

private:
    std::string name;
    std::string description;
    unsigned id;
    TaskStatus status;
    std::vector<std::string> comments;
    Staff *staff;
    TaskLogger taskLogger;
    Time taskTime;
};

#endif