#ifndef LAB6_STAFFSERVICE_H
#define LAB6_STAFFSERVICE_H

#include <string>
#include <optional>
#include "../DAL/Staff.h"
#include "../DAL/StaffRepository.h"

class StaffService {
public:
    StaffService(IStaffRepository *Repository)
    {
        this->Repository = Repository;

    }

    std::optional<unsigned> createEmployee(const std::string &name, unsigned directorID)
    {
        if (Repository->get(directorID)->getPosition() != PDirector)
            return { };
        Employee *employee = new Employee(name, Repository->getAll().size(), Repository->get(directorID));
        Repository->add(employee);
        return employee->getID();
    }

    unsigned createDirector(const std::string &name)
    {
        Director *director = new Director(name, Repository->getAll().size());
        Repository->add(director);
        return director->getID();
    }

    bool changeDirector(const unsigned ID, const unsigned newDirectorID)
    {
        if (Repository->get(ID)->getPosition() != PEmployee && Repository->get(newDirectorID)->getPosition() != PDirector)
            return false;
        return Repository->get(ID)->changeDirector(Repository->get(newDirectorID));
    }

    std::optional<std::vector<Staff*>> getSubordinate(const unsigned ID)
    {
        if (Repository->get(ID)->getPosition() != PDirector)
            return { };
        return Repository->get(ID)->getListOfSubordinates();
    }

    std::vector<Staff*> getAll() { return Repository->getAll(); }

private:
    IStaffRepository *Repository;
};


#endif