#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <optional>
#include "Racing_Simulator.h"

using namespace std;


int main() {
    Racing_Simulator race(land, 800);
    AllTerrain_boots ts;
    SpeedCamel ts1;
    cout << ts.getTime(360).value() << endl;
    cout << ts1.getTime(400).value() << endl;
    cout << endl;
    race.addTransport(ts);
    race.addTransport(ts1);
    Transport *resTs;
    auto res = race.runRace();
    if (res) {
        resTs = &res.value();
        cout << resTs;
    }


    return 0;
}
