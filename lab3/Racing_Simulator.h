#ifndef LAB_3_RACING_SIMULATOR_H
#define LAB_3_RACING_SIMULATOR_H

#include <string>
#include <vector>
#include "Land_Transport.h"
#include "Air_Transport.h"


class Racing_Simulator {
public:
    explicit Racing_Simulator(const typeOfTransport type, const double distance)
    {
        this->type = type;
        this->distance = distance;
    }

    bool addTransport(const Transport &transport)
    {
        if (this->type!=transport.getType())
            return false;
        transports.push_back(transport);
        return true;
    }

    std::optional<Transport> runRace()
    {
        if (transports.empty())
            return { };
        double minTime = -1;
        Transport resTransport;
        for (auto & transport : transports)
        {
            auto resTime = transport.getTime(distance);
            if (resTime)
                if (minTime == -1 || resTime < minTime)
                {
                    minTime = resTime.value();
                    resTransport = transport;
                }
        }
        if (minTime != -1)
            return resTransport;
        else return { };
    }

private:
    double distance;
    typeOfTransport type;
    std::vector<Transport> transports;
};


#endif