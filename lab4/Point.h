#ifndef LAB_4_POINT_H
#define LAB_4_POINT_H

#include <vector>
#include <map>
#include "File.h"

enum typePoint {
    full,
    inc
};

class Point {
public:
    Point(GlobalTime *time, const typePoint type) : createTime(time) { this->type = type; }

    void addChild(Point *point) { child = point;}
    bool checkChild()const
    {
        if (child == nullptr) return false;
        else return true;
    }

    std::optional<Point*> getParent()
    {
        if (type == inc) return parent;
        else return {};
    }
    std::map<std::string, File> getFiles()const { return files; }
    typePoint getType()const { return type; }
    unsigned getSize()const { return size; }
    int getTime()const { return 1; }

    CreateTime createTime;
protected:
    unsigned size = 0;
    typePoint type;
    std::map<std::string, File> files;
    Point *parent;
    Point *child = nullptr;
};

class FullPoint : public Point {
public:
    FullPoint(const std::map<std::string, File> &files, GlobalTime *time) : Point(time, full)
    {
        parent = nullptr;
        for (const auto & i : files)
        {
            this->files.emplace(i);
            size += i.second.getSize();
        }
    }
};

class IncrementalPoint : public Point {
public:
    IncrementalPoint(Point *parent, const std::map<std::string, File> &files, GlobalTime *time) : Point(time, inc)
    {
        parent->addChild(this);
        this->parent = parent;
        if (files.size() >= parent->getFiles().size())
        {
            for (const auto & i : files)
            {
                std::map<std::string, File> mapParent = parent->getFiles();
                auto it = mapParent.find(i.first);
                if (it == mapParent.end())
                {
                    this->files.emplace(i);
                    size += i.second.getSize();
                }
            }
        }
        else
        {
            for (const auto & i : parent->getFiles())
            {
                auto it = files.find(i.first);
                if (it == files.end())
                {
                    this->files.emplace(i);
                    size += i.second.getSize();
                }
            }
        }
    }
    IncrementalPoint(Point *parent, const std::map<std::string, File> &files, GlobalTime *time, bool copy) : Point(time, inc)
    {
        parent->addChild(this);
        this->parent = parent;
        for (const auto & i : files)
        {
            this->files.emplace(i);
            size += i.second.getSize();
        }
    }
};

#endif