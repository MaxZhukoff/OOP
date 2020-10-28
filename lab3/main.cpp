#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <optional>
#include "Racing_Simulator.h"

using namespace std;


int main() {
    double distance = 80000;

    cout << "Land Transport:" << endl;
    BactrianCamel bactrianCamel;
    SpeedCamel speedCamel;
    Centaur centaur;
    AllTerrain_boots allTerrain_boots;
    cout << bactrianCamel.getTime(distance).value() << endl;
    cout << speedCamel.getTime(distance).value() << endl;
    cout << centaur.getTime(distance).value() << endl;
    cout << allTerrain_boots.getTime(distance).value() << endl;
    cout << endl;

    cout << "Land Transport Race:" << endl;
    Racing_Simulator race_land(land, distance);
    race_land.addTransport(&bactrianCamel);
    race_land.addTransport(&speedCamel);
    race_land.addTransport(&centaur);
    race_land.addTransport(&allTerrain_boots);
    if (race_land.runRace())
        cout << race_land.runRace().value() << endl;
    cout << endl;

    cout << "Air Transport:" << endl;
    MagicCarpet magicCarpet;
    Stypa stypa;
    Broom broom;
    cout << magicCarpet.getTime(distance).value() << endl;
    cout << stypa.getTime(distance).value() << endl;
    cout << broom.getTime(distance).value() << endl;
    cout << endl;

    cout << "Air Transport Race:" << endl;
    Racing_Simulator race_air(air, distance);
    race_air.addTransport(&magicCarpet);
    race_air.addTransport(&stypa);
    race_air.addTransport(&broom);
    if (race_air.runRace())
        cout << race_air.runRace().value() << endl;
    cout << endl;

    cout << "--ALL-- Transport Race:" << endl;
    Racing_Simulator race(any, distance);
    race.addTransport(&bactrianCamel);
    race.addTransport(&speedCamel);
    race.addTransport(&centaur);
    race.addTransport(&allTerrain_boots);
    race.addTransport(&magicCarpet);
    race.addTransport(&stypa);
    race.addTransport(&broom);
    if (race.runRace())
        cout << race.runRace().value() << endl;

    return 0;
}
