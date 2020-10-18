#include "ShopSystem.h"

using namespace std;


int main() {
    ShopSystem system;

    unsigned pyaterochka = system.createShop("pyaterochka", "Lenina 85");
    unsigned perekrestok = system.createShop("Perekrestok", "Chicherina 12");
    unsigned azbuka_vkysa = system.createShop("Azbuka vkysa", "Bolshyaya posadskay 43");

    unsigned milk = system.createProduct("Milk");
    unsigned bread = system.createProduct("Bread");
    unsigned cucumber = system.createProduct("Cucumber");
    unsigned tomato = system.createProduct("Tomato");
    unsigned garlic = system.createProduct("Garlic");
    unsigned cabbage = system.createProduct("Cabbage");
    unsigned chips = system.createProduct("Chips");
    unsigned mayonnaise = system.createProduct("Mayonnaise");
    unsigned mustard = system.createProduct("Mustard");
    unsigned ketchup = system.createProduct("Ketchup");

    system.addProductToShop(pyaterochka, milk, 1, 30);
    system.addProductToShop(pyaterochka, bread, 10, 30);
    system.addProductToShop(perekrestok, milk, 10, 45);
    system.addProductToShop(perekrestok, bread, 50, 20);
    system.addProductToShop(perekrestok, cucumber, 80, 325);
    system.addProductToShop(azbuka_vkysa, tomato, 10, 45);
    system.addProductToShop(azbuka_vkysa, bread, 50, 20);
    system.addProductToShop(azbuka_vkysa, cucumber, 80, 325);
    system.addProductToShop(azbuka_vkysa, garlic, 10, 45);
    system.addProductToShop(azbuka_vkysa, cabbage, 50, 20);
    system.addProductToShop(azbuka_vkysa, chips, 50, 100);
    system.addProductToShop(azbuka_vkysa, mayonnaise, 80, 489);
    system.addProductToShop(azbuka_vkysa, mustard, 100, 350);
    system.addProductToShop(azbuka_vkysa, ketchup, 50, 500);

    std::optional<unsigned> result = system.findCheapShop(milk, 10);
    if (result) {
        system.showShop(result.value());
        cout << endl;
    }

    system.addProductToShop(pyaterochka, milk, 10, 30);

    result = system.findCheapShop(milk, 10);
    if (result) {
        system.showShop(result.value());
        cout << endl;
    }

    cout << endl;

    system.whatCanBuy(pyaterochka, 3000);
    system.whatCanBuy(azbuka_vkysa, 250);
    system.whatCanBuy(azbuka_vkysa, 10);

    cout << endl;
    vector<pair<unsigned, unsigned>> products;
    products.push_back(make_pair(milk, 10));
    products.push_back(make_pair(bread, 10));
    products.push_back(make_pair(cucumber, 1));
    std::optional<double> resultBuy = system.buyProducts(perekrestok, products);
    if (resultBuy)
        cout << "The amount of purchases is: " << resultBuy.value() << endl;
    else cout << "Can't buy" << endl;

    cout << endl;
    vector<pair<unsigned, unsigned>> products2;
    products2.push_back(make_pair(milk, 1));
    products2.push_back(make_pair(bread, 2));
    std::optional<unsigned> result2 = system.findCheapShop(products2);
    if (result2) {
        system.showShop(result2.value());
        cout << endl;
    }

    return 0;
}