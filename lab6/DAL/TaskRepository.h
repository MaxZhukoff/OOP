#ifndef LAB6_TASKREPOSITORY_H
#define LAB6_TASKREPOSITORY_H

#include <string>
#include <vector>
#include "Staff.h"
#include "Task.h"

class ITaskRepository {
public:
    virtual void add(Task *item) = 0;

    virtual void update(Task *item) = 0;

    virtual Task* get(unsigned id) = 0;

    virtual std::vector<Task*> getAll() = 0;

    ~ITaskRepository()
    {
        for (const auto & i : tasks)
            delete i;
    }

protected:
    std::vector<Task*> tasks;
};

class TaskRepository : public ITaskRepository {
public:
    void add(Task *item) override
    {
        tasks.push_back(item);
    }

    void update(Task *item) override
    {
        tasks[item->getId()] = item;
    }

    Task* get(unsigned id) override
    {
        return tasks[id];
    }

    std::vector<Task*> getAll() override { return tasks; }
};

#endif