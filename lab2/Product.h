#ifndef LAB_2_PRODUCT_H
#define LAB_2_PRODUCT_H

#include <iostream>
#include <string>
#include <map>

class Product {
public:
    Product(const std::string &name, const unsigned UID)
    {
        this->name = name;
        this->UID = UID;
    }
    friend std::ostream& operator<<(std::ostream &out, const Product &product)
    {
        out << "(ID:" << product.UID << ")" << product.name;
        return out;
    }

    bool operator<(const Product& other) const
    {
        return this->UID < other.UID;
    }

    std::string getName() {
        return this->name;
    }

    unsigned getUID() const {
        return this->UID;
    }

private:
    unsigned UID;
    std::string name;
};


#endif