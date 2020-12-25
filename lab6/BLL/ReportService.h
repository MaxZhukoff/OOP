#ifndef LAB6_REPORTSERVICE_H
#define LAB6_REPORTSERVICE_H

#include <vector>
#include "../DAL/StaffRepository.h"
#include "../DAL/TaskRepository.h"
#include "../DAL/ReportRepository.h"
#include "../DAL/GlobalTime.h"

class ReportService {
public:
    ReportService(IStaffRepository *SRepository, ITaskRepository *TRepository, IReportRepository *RRepository, GlobalTime *time)
    {
        this->time = time;
        this->SRepository = SRepository;
        this->TRepository = TRepository;
        this->RRepository = RRepository;

    }

    unsigned createDayReport(const unsigned staffID, const std::string &text) {
        DayReport *dayReport = new DayReport(RRepository->getAllReports().size(), SRepository->get(staffID), text, time);
        RRepository->addReport(dayReport);
        return dayReport->getId();
    }

    bool addTextToDayReport(const unsigned dayReportID, const std::string &text) {
        if (dayReportID > RRepository->getAllReports().size() - 1)
            return false;
        return RRepository->getReport(dayReportID)->addText(text);
    }

    bool addTaskToDayReport(const unsigned dayReportID, const unsigned taskID) {
        if (dayReportID > RRepository->getAllReports().size() - 1)
            return false;
        RRepository->getReport(dayReportID)->addTask(TRepository->get(taskID));
        return true;
    }

    void changeStateToClose(const unsigned dayReportID)const
    {
        RRepository->getReport(dayReportID)->changeStateToClose();
    }

    std::optional<std::vector<Task*>> getCompletedTaskToDayReport(const unsigned dayReportID) const {
        if (dayReportID > RRepository->getAllReports().size() - 1)
            return {};
        if (RRepository->getReport(dayReportID)->getState() == ROpen)
            return {};
        return RRepository->getReport(dayReportID)->getCompletedTasks();
    }

    unsigned createSprintReportTeam(const std::string &text) {
        SprintReportTeam *sprintReportTeam = new SprintReportTeam(RRepository->getAllReports().size(), text, time);
        RRepository->addReport(sprintReportTeam);
        return sprintReportTeam->getId();
    }

    bool closeSprintTeamReport(unsigned reportID, Staff *staff)
    {
        return RRepository->getReport(reportID)->closeSprintTeamReport(staff);
    }

    unsigned createSprintReportDirector(SprintReportTeam *sprintReportTeam, unsigned staffID, const std::string &text) {
        SprintReportDirector *sprintReportDirector = new SprintReportDirector(sprintReportTeam, RRepository->getAllReports().size(), staffID, text, time);
        RRepository->addReport(sprintReportDirector);
        return sprintReportDirector->getId();
    }

    unsigned createSprintEmployeeDirector(SprintReportDirector *sprintReportDirector, unsigned staffID, const std::string &text) {
        SprintReportEmployee *sprintReportEmployee = new SprintReportEmployee(sprintReportDirector, RRepository->getAllReports().size(), staffID, text, time);
        RRepository->addReport(sprintReportEmployee);
        return sprintReportEmployee->getId();
    }

    bool completeDirectorReport(unsigned directorReportID)
    {
        return RRepository->getReport(directorReportID)->completeReport();
    }

    bool completeEmployeeReport(unsigned employeeReportID)
    {
        return RRepository->getReport(employeeReportID)->completeReport();
    }

    bool addEmployeeReport(unsigned employeeReportID, DayReport *dayReport)
    {
        return RRepository->getReport(employeeReportID)->addReport(dayReport);
    }

    bool addDirectorReport(unsigned directorReportID, DayReport *dayReport)
    {
        return RRepository->getReport(directorReportID)->addReport(dayReport);
    }

protected:
    GlobalTime *time;
    IStaffRepository *SRepository;
    ITaskRepository *TRepository;
    IReportRepository *RRepository;

};

#endif