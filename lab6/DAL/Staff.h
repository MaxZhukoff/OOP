#ifndef LAB6_STAFF_H
#define LAB6_STAFF_H

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <optional>

class Employee;
class Director;
class TeamLead;

enum Position
{
    PTeamLead,
    PDirector,
    PEmployee
};

class Staff {
public:
    Staff(const std::string &name, const unsigned id, const Position position)
    {
        this->name = name;
        this->id = id;
        this->position = position;
    }

    unsigned getID()const { return id; }

    std::string getName()const { return name; }

    Position getPosition()const { return position; }

    virtual Staff* getDirector()const = 0;

    virtual bool changeDirector(Staff *newDirector)
    {
        if (this->getPosition() != PEmployee && newDirector->getPosition() != PDirector)
            return false;
        this->getDirector()->removeSubordinates(this);
        newDirector->addSubordinates(this);
        return true;
    }

    virtual bool addSubordinates(Staff* employee) { return false; }

    virtual bool removeSubordinates(Staff *employee) { return false; }

    virtual std::optional<std::vector<Staff*>> getListOfSubordinates()const = 0;

    bool operator<(const Staff& other) const { return this->getID() < other.getID(); }
    bool operator==(const Staff& other) const { return this->getID() == other.getID(); }

protected:
    std::string name;
    unsigned id;
    Position position;
    Staff *director;
    std::vector<Staff*> listOfSubordinates;
};

class Employee : public Staff {
public:
    Employee(const std::string &name, const unsigned id, Staff *director) : Staff(name, id, PEmployee)
    {
        this->director = director;
    }

    Staff* getDirector()const override { return director; }

    std::optional<std::vector<Staff*>> getListOfSubordinates()const override { return { }; }
};

class Director : public Staff {
public:
    Director(const std::string &name, const unsigned id) : Staff(name, id, PDirector)
    {}

    Director* getDirector()const override { return nullptr; }

    bool addSubordinates(Staff* employee) override
    {
        listOfSubordinates.push_back(employee);
        return true;
    }

    bool removeSubordinates(Staff *employee) override
    {
        auto it = find (listOfSubordinates.begin(), listOfSubordinates.end(), employee);
        if (it != listOfSubordinates.end())
        {
            listOfSubordinates.erase(it);
            return true;
        }
        else return false;
    }

    std::optional<std::vector<Staff*>> getListOfSubordinates()const override { return listOfSubordinates; }
};

class TeamLead : public Staff {
public:
    TeamLead(const std::string &name, const unsigned id, std::vector<Staff*> *staff) : Staff(name, id, PTeamLead)
    {
        this->staff = staff;
    }

    Director* getDirector()const override { return nullptr; }

    std::optional<std::vector<Staff*>> getListOfSubordinates()const override { return { }; }

private:
    std::vector<Staff*> *staff;
};

#endif