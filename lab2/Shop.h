#ifndef LAB_2_SHOP_H
#define LAB_2_SHOP_H

#include <iostream>
#include <optional>
#include <string>
#include <map>
#include "Product.h"

class Shop {
public:
    Shop(const std::string &name, const std::string &address, const unsigned UID)
    {
        this->name = name;
        this->address = address;
        this->UID = UID;
    }

    std::string getName() const {
        return this->name;
    }
    std::string getAddress() const {
        return this->address;
    }
    unsigned getUID() const {
        return this->UID;
    }

    bool addProduct(const Product &product, const unsigned &count, const double &price)
    {
        if (count < 0 || price <= 0)
            return false;
        auto it = in.emplace(product, std::make_pair(count, price));
        if (!it.second) {
            it.first->second.first += count;
            it.first->second.second = price;
        }
        return true;
    }

    bool addProduct(Product &product, const unsigned &count)
    {
        if (count < 0)
            return false;
        std::optional<double> check = this->findProduct(product, 1);
        if (check)
        {
            auto it = in.emplace(product, std::make_pair(count, 1));
            it.first->second.first += count;
            return true;
        }
        return false;
    }

    bool buyProduct(Product &product, const unsigned &count)
    {
        std::optional<double> check = this->findProduct(product, count);
        if (check)
        {
            auto it = in.emplace(product, std::make_pair(count, 1));
            it.first->second.first -= count;
            return true;
        }
        else return false;
    }

    std::optional<double> findProduct(Product &product, const unsigned count) const
    {
        auto it = in.find(product);
        if (it == in.end())
            return { };
        else if (it->second.first >= count)
        {
            return it->second.second * count;
        }
        return { };
    }

    std::optional<std::vector<std::pair<unsigned, unsigned>>> whatCanBuy(const double money)
    {
        if (money <= 0)
            return { };
        std::vector<std::pair<unsigned, unsigned>> res;
        bool resFlag = false;
        for (auto &item : in)
        {
            double price = item.second.second;
            if (price <= money)
            {
                resFlag = true;
                price = item.second.second;
                unsigned count = item.second.first;
                unsigned resCount = 0;
                while (price <= money && count-- > 0)
                {
                    price += item.second.second;
                    resCount++;
                }
                res.push_back(std::make_pair(item.first.getUID(), resCount));
            }
        }
        if (resFlag)
            return res;
        else return { };

    }

//    void showProducts(Shop &shop)
//    {
//        for (auto &item : in)
//            std::cout << item.first << "; Count: " << item.second.first << "; Price: " << item.second.second << ";" << std::endl;
//    }

    friend std::ostream& operator<<(std::ostream &out, const Shop &shop)
    {
        out << "(ID:" << shop.UID << ")" << shop.name << ", Address: \"" << shop.address << "\"";
        return out;
    }

private:
    std::string name;
    std::string address;
    unsigned UID;
    std::map<Product, std::pair<unsigned, double>> in;

};

#endif