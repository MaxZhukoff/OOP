#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Backup.h"

using namespace std;

int main() {
    GlobalTime time;
    map<string, File> files;
    File A1(&time, "A1", 100);
    File A2(&time, "A2", 50);
    File A3(&time, "A3", 150);
    files.emplace(A1.getName(), A1);
    files.emplace(A2.getName(), A2);
    files.emplace(A3.getName(), A3);

    Backup backup(files);
    backup.deleteFile(A2);
    files = backup.getFiles();
    for (const auto & i : files)
        cout << i.first << endl;
    cout << endl;

    File B1(&time, "B1", 90);
    backup.addFile(B1);
    files = backup.getFiles();
    for (const auto & i : files)
        cout << i.first << endl;
    cout << endl;


//    backup.createFullPoint(&time);
//    backup.deleteFile(A1);
//    backup.createIncrementalPoint(&time);
//    backup.createFullPoint(&time);
//    backup.addFile(A2);
//    backup.createIncrementalPoint(&time);
//
//    backup.printAllPoints();
//
//    backup.removeCountPoint(3);
//    cout << "removed:" << endl;
//    backup.printAllPoints();


//    backup.createFullPoint(&time);
//    backup.createIncrementalPoint(&time);
//
//    time.timeMachine(2021, 01, 5);
//
////    backup.createIncrementalPoint(&time);
//    backup.createFullPoint(&time);
//    backup.createIncrementalPoint(&time);
//
//    backup.printAllPoints();
//
//    backup.removeDatePoint(2020, 12, 25);
//    cout << "removed:" << endl;
//    backup.printAllPoints();


//    backup.createFullPoint(&time);
//    backup.deleteFile(A1);
//    backup.createIncrementalPoint(&time);
//    backup.createFullPoint(&time);
//    backup.addFile(A2);
//    backup.createIncrementalPoint(&time);
//
//    backup.printAllPoints();
//    backup.removeSizePoint(240);
//    cout << "removed:" << endl;
//    backup.printAllPoints();


    backup.createFullPoint(&time);
    backup.deleteFile(A1);
    backup.createIncrementalPoint(&time);
    backup.createFullPoint(&time);
    backup.addFile(A2);
    backup.createIncrementalPoint(&time);

    backup.printAllPoints();

    std::vector<std::pair<typeRemove, int>> hybridArr;
    hybridArr.push_back(std::make_pair(typeRemove::count, 3));
    hybridArr.push_back(std::make_pair(typeRemove::size, 240));

    std::pair<typeRemove, int> hybrid = backup.removeHybridPoint(hybridArr, typeHybrid::min).value();
    cout << endl << hybrid.first << " " << hybrid.second << endl;
    backup.printAllPoints();




    return 0;
}
