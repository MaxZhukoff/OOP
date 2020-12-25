#ifndef LAB6_REPORTREPOSITORY_H
#define LAB6_REPORTREPOSITORY_H

#include "Report.h"
#include "Task.h"

class IReportRepository {
public:
    virtual void addReport(Report *item) = 0;

    virtual void updateReport(Report *item) = 0;

    virtual Report* getReport(unsigned id) = 0;

    virtual std::vector<Report*> getAllReports() = 0;

    ~IReportRepository()
    {
        for (const auto & i : Reports)
            delete i;
    }

protected:
    std::vector<Report*> Reports;
};

class ReportRepository : public IReportRepository {
public:
    void addReport(Report *item) override
    {
        Reports.push_back(item);
    }

    void updateReport(Report *item) override
    {
        Reports[item->getId()] = item;
    }

    Report* getReport(unsigned id) override
    {
        return Reports[id];
    }

    std::vector<Report*> getAllReports() override { return Reports; }
};

#endif