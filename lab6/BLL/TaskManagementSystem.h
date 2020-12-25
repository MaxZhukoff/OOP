#ifndef LAB6_TASKMANAGEMENTSYSTEM_H
#define LAB6_TASKMANAGEMENTSYSTEM_H

#include <string>
#include <optional>
#include "../DAL/Task.h"
#include "../DAL/TaskRepository.h"
#include "../DAL/StaffRepository.h"
#include "../DAL/GlobalTime.h"

class TaskManagementSystem {
public:
    TaskManagementSystem(ITaskRepository *TRepository, IStaffRepository *SRepository, GlobalTime *time)
    {
        this->SRepository = SRepository;
        this->TRepository = TRepository;
        this->time = time;
    }

    std::optional<unsigned> createTask(const std::string &name, const std::string &description, const unsigned staffID)
    {
        if (staffID > SRepository->getAll().size() - 1)
            return { };
        Task *task = new Task(name, description, SRepository->get(staffID), time, TRepository->getAll().size());
        TRepository->add(task);
        return task->getId();
    }

    bool changeStatus(const unsigned taskID, const unsigned staffID, TaskStatus status)
    {
        if (taskID > TRepository->getAll().size() - 1 || staffID > SRepository->getAll().size() - 1)
            return false;
        TRepository->get(taskID)->changeStatus(status, SRepository->get(staffID));
        return true;
    }

    bool changeStaff(const unsigned taskID, const unsigned staffID)
    {
        if (taskID > TRepository->getAll().size() - 1 || staffID > SRepository->getAll().size() - 1)
            return false;
        TRepository->get(taskID)->changeStaff(SRepository->get(staffID));
        return true;
    }

    bool addComment(const unsigned taskID, const unsigned staffID, const std::string &comment)
    {
        if (taskID > TRepository->getAll().size() - 1 || staffID > SRepository->getAll().size() - 1)
            return false;
        if (TRepository->get(taskID)->getStatus() == Resolved)
            return false;
        TRepository->get(taskID)->addComment(comment, SRepository->get(staffID));
        return true;
    }

    std::vector<Task*> getStaffTasks(const unsigned staffID)
    {
        std::vector<Task*> staffTasks;
        for (const auto & i : TRepository->getAll())
        {
            if (i->getStaff()->getID() == staffID)
                staffTasks.push_back(i);
        }
        return staffTasks;
    }

    bool FidTaskById(const unsigned id)const
    {
        if (id > TRepository->getAll().size() - 1)
            return false;
        else return true;
    }

    std::optional<unsigned> FindByLastChange()
    {
        if (TRepository->getAll().empty())
            return { };
        unsigned taskID;
        int timeChange = 0;
        for (const auto & i : TRepository->getAll())
        {
            int time1 = i->getLogger()->getAll()[i->getLogger()->getAll().size() - 1]->getTime()->getSecFrom();
            if (i->getLogger()->getAll()[i->getLogger()->getAll().size() - 1]->getTime()->getSecFrom() > timeChange)
            {
                timeChange = time1;
                taskID = i->getId();
            }
        }
        return taskID;
    }

    std::optional<unsigned> FindByCreate()
    {
        if (TRepository->getAll().empty())
            return { };
        unsigned taskID;
        time->upDate();
        int timeChange = time->getSecFrom();
        for (const auto & i : TRepository->getAll())
        {
            int time1 = i->getLogger()->getAll()[0]->getTime()->getSecFrom();
            if (i->getLogger()->getAll()[0]->getTime()->getSecFrom() < timeChange)
            {
                timeChange = time1;
                taskID = i->getId();
            }
        }
        return taskID;
    }

    std::optional<unsigned> FindByAssignedStaff(const unsigned staffID)
    {
        for (const auto & i : TRepository->getAll())
        {
            if (i->getStaff()->getID() == staffID)
                return i->getId();
        }
        return { };
    }

    std::vector<Task*> FindByStaffChanges(const unsigned staffID)
    {
        std::vector<Task*> staffChanges;
        for (const auto & i : TRepository->getAll())
        {
            auto res = i->getLogger()->findStaff(SRepository->get(staffID));
            if (res)
                staffChanges.push_back(i);
        }
        return staffChanges;
    }

private:
    IStaffRepository *SRepository;
    ITaskRepository *TRepository;
    GlobalTime *time;
};

#endif