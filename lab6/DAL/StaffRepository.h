#ifndef LAB6_STAFFREPOSITORY_H
#define LAB6_STAFFREPOSITORY_H

#include <string>
#include <map>
#include <optional>
#include "Staff.h"

class IStaffRepository {
public:
    IStaffRepository(const std::string &TeamLeadName)
    {
        TeamLead *teamLead = new TeamLead(TeamLeadName, 0, &staff);
        staff.push_back(teamLead);
    }

    virtual void add(Staff *item) = 0;

    virtual void update(Staff *item) = 0;

    virtual Staff* get(unsigned id) = 0;

    virtual std::vector<Staff*> getAll() = 0;

    ~IStaffRepository()
    {
        for (const auto & i : staff)
            delete i;
    }

protected:
    std::vector<Staff*> staff;
};

class StaffRepository : public IStaffRepository {
public:
    StaffRepository(const std::string &TeamLeadName) : IStaffRepository(TeamLeadName)
    {}

    void add(Staff *item) override
    {
        staff.push_back(item);
    }

    void update(Staff *item) override
    {
        staff[item->getID()] = item;
    }

    Staff* get(unsigned id) override
    {
        return staff[id];
    }

    std::vector<Staff*> getAll() override { return staff; }
};

#endif