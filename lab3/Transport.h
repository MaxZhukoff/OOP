#ifndef LAB_3_TRANSPORT_H
#define LAB_3_TRANSPORT_H

#include <string>
#include <cmath>
#include <optional>

enum typeOfTransport {
    land,
    air,
    any
};

class Transport {
public:
    virtual std::optional<double> getTime(const double distance) const
    {
        if (distance <= 0 || speed <= 0)
            return { };
        else return distance / speed;
    }
    typeOfTransport getType() const
    {
        return type;
    }

    std::string getName()const
    {
        return name;
    }

    friend std::ostream& operator<<(std::ostream &out, const Transport &transport)
    {
        out << transport.getName();
        return out;
    }

    bool operator<(const Transport& other) const
    {
        return this->name < other.name;
    }

protected:
    std::string name;
    typeOfTransport type = any;
    double speed = 1; //>0;
};


#endif