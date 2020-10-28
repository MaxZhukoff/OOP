#ifndef LAB_3_LAND_TRANSPORT_H
#define LAB_3_LAND_TRANSPORT_H

#include <vector>
#include "Transport.h"

class Land_Transport : public Transport {
public:
    Land_Transport()
    {
        type = land;
    }
    std::optional<double> getTime(const double distance) const override
    {
        if (distance <= 0 || speed <= 0 || restInterval <= 0)
            return { };
        double travelTime = distance / speed;
        int restCount = ceil(travelTime) / restInterval;
        if ((int)ceil(distance/speed)%restInterval == 0)
            restCount--;
        double totalRestTime = 0;
        if (!restTime.empty())
        {
            int restIndex = restTime.size() - 1;
            while (restCount-- > 0)
            {
                double restTimeNow = restTime[restTime.size() - restIndex - 1];
                if (restTimeNow < 0)
                    return { };
                totalRestTime += restTimeNow;
                if (restIndex > 0)
                    restIndex--;
            }
        }
        double totalTime = travelTime + totalRestTime;
        return totalTime;
    }

protected:
    int restInterval;
    std::vector<double> restTime;
};

class BactrianCamel : public Land_Transport {
public:
    BactrianCamel() {
        name = "BactrianCamel";
        speed = 10;
        restInterval = 30;
        restTime.push_back(5);
        restTime.push_back(8);
    }
};

class SpeedCamel : public Land_Transport {
public:
    SpeedCamel() {
        name = "SpeedCamel";
        speed = 40;
        restInterval = 10;
        restTime.push_back(5);
        restTime.push_back(6.5);
        restTime.push_back(8);
    }
};

class Centaur : public Land_Transport {
public:
    Centaur() {
        name = "Centaur";
        speed = 15;
        restInterval = 8;
        restTime.push_back(2);
    }
};

class AllTerrain_boots : public Land_Transport {
public:
    AllTerrain_boots() {
        name = "AllTerrain_boots";
        speed = 6;
        restInterval = 60;
        restTime.push_back(10);
        restTime.push_back(5);
    }
};

#endif