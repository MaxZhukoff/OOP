#ifndef LAB_4_FILE_H
#define LAB_4_FILE_H

#include <string>
#include "Time.h"

class File {
public:
    File(GlobalTime *time, const std::string &name, const unsigned size) : createDate(time)
    {
        this->name = name;
        this->size = size;
    }

    std::string getName()const { return name; }
    unsigned getSize()const { return size; }
    CreateTime* getCreateDate() { return &createDate; }

    friend bool operator==(const File &file1, const File &file2)
    {
        if (file1.getName() == file2.getName() && file1.getSize() == file2.getSize() && file1.createDate.getCreationSecFrom() == file2.createDate.getCreationSecFrom())
            return true;
        else return false;
    }

private:
    std::string name;
    unsigned size;
    CreateTime createDate;
};

#endif