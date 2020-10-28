#ifndef LAB_3_RACING_SIMULATOR_H
#define LAB_3_RACING_SIMULATOR_H

#include <string>
#include <algorithm>
#include <vector>
#include "Land_Transport.h"
#include "Air_Transport.h"


class Racing_Simulator {
public:
    explicit Racing_Simulator(const typeOfTransport type, const double distance)
    {
        this->distance = distance;
        this->type = type;
    }

    bool addTransport(Transport *transport)
    {
        if (type != transport->getType() && type != any || std::find(transports.begin(), transports.end(), transport) != transports.end())
            return false;
        transports.push_back(transport);
        return true;
    }

    std::optional<std::string> runRace()
    {
        if (transports.empty())
            return { };
        double minTime = -1;
        std::string resTsName;
        for (auto & transport : transports)
        {
            std::optional<double> resTime = transport->getTime(distance);
            if (resTime)
                if (minTime == -1 || resTime < minTime)
                {
                    minTime = resTime.value();
                    resTsName = transport->getName();
                }
        }
        if (minTime != -1)
            return resTsName;
        else return { };
    }

    void changeDistance(const double distance)
    {
        if (distance > 0) this->distance = distance;
    }

    void changeTypeOfRace(const typeOfTransport type)
    {
        this->type = type;
        transports.clear();
    }


private:
    double distance;
    typeOfTransport type;
    std::vector<Transport*> transports;
};


#endif