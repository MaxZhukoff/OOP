#include <iostream>
#include <vector>
#include "BLL/ReportService.h"
#include "BLL/StaffService.h"
#include "BLL/TaskManagementSystem.h"
#include "DAL/GlobalTime.h"


int main() {
    GlobalTime time;
    StaffRepository staffRepository("Tim");
    StaffService staffService(&staffRepository);

    unsigned directorDynya = staffService.createDirector("Dynya");
    unsigned employeeJora = staffService.createEmployee("Jora", directorDynya).value();
    unsigned employeeFedya = staffService.createEmployee("Fedya", directorDynya).value();

    std::vector<Staff*> staff = staffService.getAll();
    for (const auto & i : staff)
        std::cout << i->getName() << " " << i->getPosition() << std::endl;
    std::cout << std::endl;


    TaskRepository taskRepository;
    TaskManagementSystem taskManagementSystem(&taskRepository, &staffRepository, &time);
    unsigned DynyaTask1 = taskManagementSystem.createTask("Dynya task1", "Write 'Hello World'", directorDynya).value();
    unsigned DynyaTask2 = taskManagementSystem.createTask("Dynya task2", "Write more 'Hello World'", directorDynya).value();
    unsigned ForaTask1 = taskManagementSystem.createTask("Jora task1", "Write 'Hello World'", employeeJora).value();
    unsigned FedyaTask1 = taskManagementSystem.createTask("Fedya task1", "Write 'Hello World'", employeeFedya).value();

    std::cout << taskManagementSystem.FidTaskById(DynyaTask1) << std::endl;
    std::cout << std::endl;
    std::vector<Task*> tasks = taskManagementSystem.getStaffTasks(directorDynya);
    for (const auto & i : tasks)
        std::cout << "Task name: '" << i->getName() << "' Staff: " << i->getStaff()->getName() << std::endl;
    std::cout << std::endl;

    ReportRepository reportRepository;
    ReportService reportService(&staffRepository, &taskRepository, &reportRepository, &time);

    taskManagementSystem.changeStatus(DynyaTask1, directorDynya, Resolved);
    taskManagementSystem.changeStatus(DynyaTask2, directorDynya, Resolved);
    taskManagementSystem.changeStatus(ForaTask1, employeeJora, Resolved);
    taskManagementSystem.changeStatus(FedyaTask1, employeeFedya, Resolved);

    unsigned DynyaDayReport1 = reportService.createDayReport(DynyaTask1, "Day 1 Report");
    reportService.addTaskToDayReport(DynyaDayReport1, DynyaTask1);
    reportService.addTaskToDayReport(DynyaDayReport1, DynyaTask2);
    reportService.changeStateToClose(DynyaDayReport1);

    std::cout << "DynyaReport:" << std::endl;
    std::vector<Task*> tasks1 = reportService.getCompletedTaskToDayReport(DynyaDayReport1).value();
    for (const auto & i : tasks1)
        std::cout << "Task name: '" << i->getName() << "' Staff: " << i->getStaff()->getName() << std::endl;






    return 0;
}
