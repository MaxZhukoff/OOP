#ifndef LAB_2_SHOPSYSTEM_H
#define LAB_2_SHOPSYSTEM_H

#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include "Product.h"
#include "Shop.h"

class ShopSystem {
public:
    ShopSystem()
    {}

    unsigned createShop(const std::string &name, const std::string &address) {
        shops.push_back(Shop(name, address, shops.size()));
        return shops.back().getUID();
    }

    unsigned createProduct(const std::string &name) {
        products.push_back(Product(name, products.size()));
        return products.back().getUID();
    }

    bool showShop(const unsigned shopUID) const
    {
        if (shops.size() - 1 < shopUID)
            return false;
        else {
            std::cout << shops[shopUID];
            return true;
        }
    }

    bool addProductToShop(const unsigned shopUID , const unsigned productUID, const unsigned &count, const double &price)
    {
        if (shops.size() - 1 < shopUID || products.size() - 1 < productUID || count < 1 || price <= 0)
            return false;
        if (shops[shopUID].addProduct(products[productUID], count, price))
            return true;
        else
            return false;
    }

     std::optional<unsigned> findCheapShop(const unsigned productUID, const unsigned count = 1)
     {
        if (products.size() - 1 < productUID || count < 1)
             return { };
        std::optional<double> minPrice = std::numeric_limits<double>::max();
        std::optional<double> check;
        unsigned shopUID;
        for (auto &item : shops)
        {
            check = item.findProduct(products[productUID], count);
            if (check && check < minPrice)
            {
                minPrice = check;
                shopUID = item.getUID();
            }
        }
        if (minPrice == std::numeric_limits<double>::max())
            return { };
        else return shopUID;
     }

    std::optional<unsigned> findCheapShop(const std::vector<std::pair<unsigned, unsigned>> &items) {
        std::vector<std::pair<unsigned, double>> cheapShops;
        for (auto &shIt : shops)
        {
            unsigned shopUID = shIt.getUID();
            double sum = 0;
            for (auto &it : items)
            {
                std::optional<double> check = shIt.findProduct(products[it.first], it.second);
                if (check)
                {
                    sum += check.value();
                }
                else break;
            }
            if (sum != 0)
                cheapShops.push_back(std::make_pair(shopUID, sum));
        }
        unsigned minShopUID;
        double minSum = std::numeric_limits<double>::max();
        if (cheapShops.empty())
            return { };
        for (auto &it : cheapShops)
        {
            if (it.second < minSum)
            {
                minSum = it.second;
                minShopUID = it.first;
            }
        }
        return minShopUID;
    }

    std::optional<std::vector<std::pair<unsigned, unsigned>>> whatCanBuy(const unsigned shopUID, const double money)
     {
         if (shops.size() - 1 < shopUID || money <= 0)
             return { };
         if (shops[shopUID].whatCanBuy(money))
             return shops[shopUID].whatCanBuy(money);
         else return { };
     }

    std::optional<double> buyProducts(const unsigned shopUID, const std::vector<std::pair<unsigned, unsigned>> &items)
    {
        if (shops.size() - 1 < shopUID)
            return { };
        std::vector<std::pair<Product, unsigned>> bought;
        double resPrice = 0;
        std::optional<double> result;
        if (!items.empty()) {
            for (auto &it : items) {
                if (products.size() - 1 < it.first || it.second < 1)
                    return { };
                result = shops[shopUID].findProduct(products[it.first], it.second);
                if (result) {
                    resPrice += result.value();
                    bought.push_back(std::make_pair(products[it.first], it.second));
                } else return { };
            }
            for (auto &it : bought) {
                shops[shopUID].buyProduct(it.first, it.second);
            }
            return resPrice;
        } else return { };
    }

//    void showAllProducts() const
//    {
//        std::cout << "--------Products:----------" << std::endl;
//        for (auto& it : products)
//            std::cout << it << std::endl;
//        std::cout << std::endl;
//    }
//
//    void showAllShops() const
//    {
//        std::cout << "--------Shops:-------------" << std::endl;
//        for (auto& it : shops)
//            std::cout << it << std::endl;
//        std::cout << std::endl;
//    }
//    void showShopInfo(const unsigned shopUID) {
//        std::cout << shops[shopUID] << "; Products: " << std::endl;
//        shops[shopUID].showProducts(shops[shopUID]);
//        std::cout << std::endl;
//    }

private:
    std::vector<Product> products;
    std::vector<Shop> shops;
};


#endif //LAB_2_SHOPSYSTEM_H