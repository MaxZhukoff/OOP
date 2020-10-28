#ifndef LAB_3_AIR_TRANSPORT_H
#define LAB_3_AIR_TRANSPORT_H

#include <vector>
#include "Transport.h"

class Air_Transport : public Transport {
public:
    Air_Transport()
    {
        type = air;
    }

    std::optional<double> getTime(const double distance) const override
    {
        if (distance <= 0 || speed <= 0)
            return { };
        double distanceReducerTotal = 0;
        double distanceReducerPercent = distanceReducerInPercents(distance);
        if (distanceReducerPercent != 0)
            distanceReducerTotal = distance / 100 * distanceReducerPercent;
        double distanceTotal = distance - distanceReducerTotal;
        double travelTime = distanceTotal / speed;
        return travelTime;
    }

protected:
    virtual double distanceReducerInPercents(const double distance)const
    {
        int indexCoefficient = 0;
        if (distanceReducer.size() > 1)
        {
            while (distance >= distanceReducer[indexCoefficient].first)
                if (indexCoefficient + 1 <= distanceReducer.size() - 1)
                    indexCoefficient++;
                else return distanceReducer[indexCoefficient].second;
            return distanceReducer[indexCoefficient].second;
        }
        else if (distanceReducer.size() == 1)
            return distanceReducer[indexCoefficient].second;
        else return 0;
    }

    std::vector<std::pair<double, double>> distanceReducer;
};

class MagicCarpet : public Air_Transport {
public:
    MagicCarpet() {
        name = "MagicCarpet";
        speed = 10;
        distanceReducer.emplace_back(1000, 0);
        distanceReducer.emplace_back(5000, 3);
        distanceReducer.emplace_back(10000, 10);
        distanceReducer.emplace_back(10000, 5);
    }
};

class Stypa : public Air_Transport {
public:
    Stypa() {
        name = "Stypa";
        speed = 8;
        distanceReducer.emplace_back(0, 6);
    }
};

class Broom : public Air_Transport {
public:
    Broom() {
        name = "Broom";
        speed = 20;
    }

private:
    double distanceReducerInPercents(const double distance)const override
    {
        double TotalDistance = distance;
        double distanceReducerPercent = 0;
        for (int distanceLeft = (int)ceil(distance); distanceLeft >= 1000; distanceLeft -= 1000)
            TotalDistance -= TotalDistance / 100;
        distanceReducerPercent = 100 - (TotalDistance / distance * 100);
        return distanceReducerPercent;
    }
};

#endif