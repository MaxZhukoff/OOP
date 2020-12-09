#ifndef LAB_4_BACKUP_H
#define LAB_4_BACKUP_H

#include <iostream>
#include <string>
#include <optional>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include "File.h"
#include "Point.h"

enum typeHybrid {
    min,
    max
};

enum typeRemove {
    count,
    date,
    size
};

class Backup {
public:
    Backup(const std::map<std::string, File> &files)
    {
        for (const auto & i : files)
            this->files.emplace(i);
    }

    Backup(Backup &backup)
    {
        for (const auto & i : backup.points)
        {
            GlobalTime time;
            time.timeMachine(i->createTime.getCreationSecFrom());
            typePoint type = i->getType();
            if (type == full)
            {
                FullPoint *fullPoint = new FullPoint(i->getFiles(), &time);
                points.push_back(fullPoint);
            }
            else if (type == inc)
            {
                IncrementalPoint *incrementalPoint = new IncrementalPoint(points[points.size() - 1], i->getFiles(), &time, true);
                points.push_back(incrementalPoint);
            }
        }
    }

    void addFile(const File &file)
    {
        auto it = files.find(file.getName());
        if (it == files.end())
            files.emplace(file.getName(), file);
        else
        {
            std::cout << "Such file exists, replace it? (y/n):";
            char answer;
            std::cin >> answer;
            if (answer == 'y')
            {
                files.erase(it);
                files.emplace(file.getName(), file);
            }
        }
    }

    void addFile(const std::map<std::string, File> &files)
    {
        for (const auto & i : files)
        {
            auto it = files.find(i.first);
            if (it == files.end())
                this->files.emplace(i.first, i.second);
            else
            {
                std::cout << "Such file exists, replace it? (y/n):";
                char answer;
                std::cin >> answer;
                if (answer == 'y')
                {
                    this->files.erase(it);
                    this->files.emplace(i.first, i.second);
                }
            }
        }
    }

    bool deleteFile(const File &file)
    {
        auto it = files.find(file.getName());
        if (it != files.end())
        {
            files.erase(it);
            return true;
        }
        else return false;
    }

    unsigned getSize()const
    {
        unsigned size = 0;
        for (const auto & i : files)
            size += i.second.getSize();
        for (const auto & i : points)
            size += i->getSize();
        return size;
    }
    unsigned getSizePoints()const
    {
        unsigned size = 0;
        for (const auto & i : points)
            size += i->getSize();
        return size;
    }

    std::map<std::string, File> getFiles()const { return files; }

    std::optional<FullPoint> createFullPoint(GlobalTime *time)
    {
        FullPoint *fullPoint = new FullPoint(files, time);
        points.push_back(fullPoint);
        return *fullPoint;
    }

    std::optional<IncrementalPoint> createIncrementalPoint(GlobalTime *time)
    {
        if (points.empty())
            return {};
        IncrementalPoint *incrementalPoint = new IncrementalPoint(points[points.size() - 1], files, time);
        points.push_back(incrementalPoint);
        return *incrementalPoint;
    }

    void printAllPoints()const
    {
        for (const auto & i : points)
        {
            if (i->getType() == full)
                std::cout << "Full Point" << " (size = " << i->getSize() << "):" << std::endl;
            else
                std::cout << "Inc Point" << " (size = " << i->getSize() << "):" << std::endl;
            for (const auto & j : i->getFiles())
                std::cout << j.first << " " << j.second.getSize() << std::endl;
        }
        std::cout << std::endl;
    }

    int removeCountPoint(const unsigned count)
    {
        bool canRemove;
        unsigned needToRemove = 0;
        unsigned indexRemove;
        if (points.size() >= count)
        {
            for (unsigned i = 0; i < count; i++)
            {
                canRemove = true;
                indexRemove = i;
                if (points[indexRemove]->checkChild() && indexRemove + 1 < count)
                {
                    indexRemove++;
                    while (indexRemove < points.size() && points[indexRemove]->checkChild())
                    {
                        if (indexRemove + 1 >= count || indexRemove + 1 >= points.size() || points[indexRemove + 1]->getType() == full)
                            canRemove = false;
                        indexRemove++;
                    }
                }
                else canRemove = false;
                if (canRemove)
                {
                    for (unsigned j = i; j <= indexRemove; j++, needToRemove++)
                        delete points[j];
                    i = indexRemove;
                }
                else break;
            }
            points.erase(points.begin(), points.begin() + needToRemove);
                std::cout << needToRemove << " was removed instead of " << count << std::endl;
            return needToRemove;
        }
        else
        {
            std::cout << "Unable to delete " << count << " items" << std::endl;
            return 0;
        }
    }

    bool removeDatePoint(const int year, const int month, const int day)
    {
        GlobalTime localTime;
        localTime.timeMachine(year, month, day);
        int sec = localTime.getSecFrom();
        bool canRemove;
        unsigned needToRemove = 0;
        unsigned indexRemove;
        for (unsigned i = 0; i < points.size(); i++) {
            canRemove = true;
            indexRemove = i;
            if (points[indexRemove]->createTime.getCreationSecFrom() > sec)
                break;
            else if (points[indexRemove]->checkChild())
            {
                indexRemove++;
                while (indexRemove < points.size() && points[indexRemove]->checkChild()) {
                    if (indexRemove + 1 >= points.size() || points[indexRemove + 1]->getType() == full || points[indexRemove + 1]->createTime.getCreationSecFrom() > sec)
                        canRemove = false;
                    indexRemove++;
                }
            }
            if (canRemove)
            {
                for (unsigned j = i; j <= indexRemove; j++, needToRemove++)
                    delete points[j];
                i = indexRemove;
            } else break;
        }
        points.erase(points.begin(), points.begin() + needToRemove);
        std::cout << "Removed " << needToRemove << " points" << std::endl;
        return needToRemove;
    }

    int removeSizePoint(const unsigned size)
    {
        unsigned sumSize = this->getSizePoints();
        if (sumSize < size)
            return 0;
        int indexRemove;
        int maxChildIndex = 0;
        std::set<unsigned> index;
        for (int i = 0; i < points.size(); i++)
        {
            if (sumSize <= size)
                break;
            indexRemove = i;
            if (points[indexRemove]->getType() == full && points[indexRemove]->checkChild())
            {
                indexRemove++;
                while (indexRemove < points.size())
                {
                    if (points[indexRemove]->checkChild())
                        indexRemove++;
                    else {
                        break;
                    }
                }
                maxChildIndex = indexRemove;
                for (indexRemove; indexRemove >= i; --indexRemove)
                {
                    if (size < sumSize)
                    {
                        index.emplace(indexRemove);
                        sumSize -= points[indexRemove]->getSize();
                    }
                    else break;
                }
                i = maxChildIndex;
            }
            else
            {
                index.emplace(indexRemove);
                sumSize -= points[indexRemove--]->getSize();
            }
        }
        for (const auto & i : index)
            delete points[i];
        auto it = points.begin();
        for (const auto & i : index)
            points.erase(it-- + i);
        std::cout << "Removed " << index.size() << " points" << std::endl;
        return index.size();
    }

    std::optional<std::pair<typeRemove, int>> removeHybridPoint(std::vector<std::pair<typeRemove, int>> &types,const typeHybrid typeH, const int year = -1, const int month = -1, const int day = -1)
    {
        int valueOfCount;
        int valueOfSize;
        if (types.size() < 2)
            return {};
        std::vector<std::pair<typeRemove, int>> removeType;
        for (const auto & i : types)
        {
            if (i.second < 0)
                return {};
            if (i.first == count)
            {
                Backup copy(*this);
                valueOfCount = i.second;
                std::cout << "Backup shadow for deletion by count: ";
                removeType.push_back(std::make_pair(i.first, copy.removeCountPoint(i.second)));

            }
            else if (i.first == date)
            {
                if (year <= -1 || month <= -1 || day <= -1)
                    return {};
                Backup copy(*this);
                std::cout << "Backup shadow for deletion by date: ";
                removeType.push_back(std::make_pair(i.first, copy.removeDatePoint(year, month, day)));
            }
            else if (i.first == size)
            {
                valueOfSize = i.second;
                Backup copy(*this);
                std::cout << "Backup shadow for deletion by size: ";
                removeType.push_back(std::make_pair(i.first, copy.removeSizePoint(i.second)));

            }
        }
        if (typeH == min)
        {
            std::pair<typeRemove, int> minType = std::make_pair(removeType[0].first, removeType[0].second);
            for (const auto & i : removeType)
            {
                if (i.second < minType.second)
                {
                    minType.first = i.first;
                    minType.second = i.second;
                }
            }
            if (minType.first == count)
                this->removeCountPoint(valueOfCount);
            else if (minType.first == date)
                this->removeDatePoint(year, month, day);
            else if (minType.first == size)
                this->removeSizePoint(valueOfSize);
            return minType;
        }
        else if (typeH == max)
        {
            std::pair<typeRemove, int> maxType = std::make_pair(removeType[0].first, removeType[0].second);
            for (const auto & i : removeType)
            {
                if (i.second > maxType.second)
                {
                    maxType.first = i.first;
                    maxType.second = i.second;
                }
            }
            if (maxType.first == count)
            this->removeCountPoint(valueOfCount);
            else if (maxType.first == date)
                this->removeDatePoint(year, month, day);
            else if (maxType.first == size)
                this->removeSizePoint(valueOfSize);
            return maxType;
        }
        return {};
    }

    ~Backup()
    {
        for (const auto & i : points) {
            delete i;
        }
    }

private:
    std::map<std::string, File> files;
    std::vector<Point*> points;
};


#endif