#ifndef LAB6_REPORT_H
#define LAB6_REPORT_H

#include <vector>
#include <string>
#include "Staff.h"
#include "Task.h"
#include "GlobalTime.h"

class SprintReportTeam;
class SprintReportDirector;
class DayReport;
class SprintReport;

enum ReportState {
    ROpen,
    RClose
};

class Report {
public:
    Report(const unsigned id, const std::string &text, GlobalTime *time) : reportTime(time)
    {
        this->id = id;
        state = ROpen;
        this->time = time;
    }

    virtual bool addText(const std::string &text)
    {
        if (state == RClose)
            return false;
        this->text.push_back(text);
        return true;
    }

    virtual bool addTask(Task *task)
    {
        if (state == RClose || task->getStatus() != Resolved)
            return false;
    }

    virtual bool addReport(DayReport *dayReport) { return false; }


    virtual bool completeReport() { return false; }

    virtual std::optional<std::vector<Task*>> getCompletedTasks()const { return { }; }

    ReportState getState()const { return state; }

    virtual void changeStateToClose() { }

    bool closeSprintTeamReport(Staff *staff) { return false;}

    unsigned getId()const { return id; }

protected:
    unsigned id;
    ReportState state;
    GlobalTime *time;
    Time reportTime;
    std::vector<std::string> text;
};

class DayReport : public Report {
public:
    DayReport(const unsigned id, Staff *staff, const std::string &text, GlobalTime *time) : Report(id, text, time)
    {
        this->staff = staff;
        state = ROpen;
    }

    bool addTask(Task *task) override
    {
        if (state == RClose || task->getStatus() != Resolved)
            return false;
        tasks.push_back(task);
        return true;
    }

    void changeStateToClose() override { state = RClose; }

    std::optional<std::vector<Task*>> getCompletedTasks()const override { return tasks; }

    Time getTimeCreated()const { return reportTime; }

    Staff* getStaff()const { return staff; }


private:
    Staff *staff;
    std::vector<Task*> tasks;
};

class SprintStateDraft {
public:
    SprintStateDraft(Report *report)
    {
        this->sprintReport = report;
        open = true;
    }

    bool addDayReport(DayReport *dayReport)
    {
        if (dayReport->getState() != RClose)
            return false;
        reports.push_back(dayReport);
        return true;
    }

    std::optional<std::vector<DayReport*>> closeDraft()
    {
        if (open) {
            open = false;
            return reports;
        }
        else return { };
    }

    bool getOpen()const { return open; }

protected:
    std::vector<DayReport*> reports;
    bool open;
    Report *sprintReport;
};

class SprintReportTeam : public Report {
public:
    SprintReportTeam(const unsigned id, const std::string &text, GlobalTime *time) : Report(id, text, time)
    {}

    void addDirectorReport(Staff *staffReport, std::map<Staff*, std::vector<DayReport*>*> &staffReports)
    {
        reports.emplace(staffReport, staffReports);
    }

    bool closeSprintTeamReport(Staff *staff)
    {
        if (staff->getPosition() == PTeamLead)
        {
            state = RClose;
            return true;
        }
        else return false;
    }

private:
    std::map<Staff*, std::map<Staff*, std::vector<DayReport*>*>> reports;
};

class SprintReportDirector : public Report {
public:
    SprintReportDirector(SprintReportTeam *sprintReportTeam, const unsigned id, const unsigned staffId, const std::string &text, GlobalTime *time) : Report(id, text, time), sprintState(this)
    {
        this->sprintReportTeam = sprintReportTeam;
    }

    bool completeReport() override
    {
        auto res = sprintState.closeDraft();
        if (!res)
            return false;
        reports.emplace(staff, &res.value());
        sprintReportTeam->addDirectorReport(staff, reports);
        return true;
    }

    bool addReport(DayReport *dayReport) override
    {
        return sprintState.addDayReport(dayReport);
    }

    void addStaffReport(Staff *staffReport, std::vector<DayReport*> *staffReports)
    {
        reports.emplace(staffReport, staffReports);
    }

private:
    SprintReportTeam *sprintReportTeam;
    Staff *staff;
    std::map<Staff*, std::vector<DayReport*>*> reports;
    SprintStateDraft sprintState;
};

class SprintReportEmployee : public Report {
public:
    SprintReportEmployee(SprintReportDirector *sprintReportDirector, const unsigned id, const unsigned staffId, const std::string &text, GlobalTime *time) : Report(id, text, time), sprintState(this)
    {
        this->sprintReportDirector = sprintReportDirector;
    }

    bool completeReport() override
    {
        auto res = sprintState.closeDraft();
        if (!res)
            return false;
        std::vector<DayReport*> reports;
        reports = res.value();
        sprintReportDirector->addStaffReport(staff, &reports);
        return true;
    }

    bool addReport(DayReport *dayReport) override
    {
        return sprintState.addDayReport(dayReport);
    }

private:
    SprintReportDirector *sprintReportDirector;
    Staff *staff;
    SprintStateDraft sprintState;
};

#endif