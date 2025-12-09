#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

#include "3cmd.h"

using namespace std;

map<string, set<string>> planeToTowns; 
map<string, set<string>> townToPlanes; 

void createPlane(const vector<string>& args) {
    if (args.size() < 4) {
        cerr << "Ошибка: Малое количество аргументов\n";
        return;
    }

    const string plane = args[1];
    set<string> towns;
    for (int i = 2; i < args.size(); ++i) {
        towns.insert(args[i]);
    }

    if (towns.size() < 2) {
        cerr << "Ошибка: 1 Город\n";
        return;
    }

    auto old = planeToTowns.find(plane);
    if (old != planeToTowns.end()) {
        for (string town : old->second) {
            townToPlanes[town].erase(plane);
            if (townToPlanes[town].empty()) {
                townToPlanes.erase(town);
            }
        }
    }


    planeToTowns[plane] = towns;
    for (string town : towns) {
        townToPlanes[town].insert(plane);
    }
}

void planesForTown(const vector<string>& args) {
    if (args.size() != 2) {
        cerr << "Ошибка: Неверное количество аргументов\n";
        return;
    }

    string town = args[1];
    auto it = townToPlanes.find(town);
    if (it == townToPlanes.end() || it->second.empty()) {
        cout << "Нет самолетов\n";
        return;
    }

    bool first = true;
    for (string plane : it->second) {
        if (!first) cout << " ";
        cout << plane;
        first = false;
    }
    cout << "\n";
}

void townsForPlane(const vector<string>& args) {
    if (args.size() != 2) {
        cerr << "Ошибка: Неверное количество аргументов\n";
        return;
    }

    string plane = args[1];
    auto it_plane = planeToTowns.find(plane);
    const set<string>& towns = it_plane->second;

    for (string town : towns) {
        cout << town << ": ";

        auto it_town = townToPlanes.find(town);
        if (it_town == townToPlanes.end()) {
            cout << "0\n";
            continue;
        }

        set<string> planesInTown = it_town->second;
        bool first = true;
        for (const string& p : planesInTown) {
            if (p == plane) continue;
            if (!first) cout << " ";
            cout << p;
            first = false;
        }

        if (first) {
            cout << "0";
        }
        cout << "\n";
    }
}

void listPlanes() {
    if (planeToTowns.empty()) {
        cout << "Пусто\n";
        return;
    }

    for (auto i : planeToTowns) {
        string plane = i.first;
        set<string> towns = i.second;

        cout << "PLANE " << plane << ":";
        for (string town : towns) {
            cout << " " << town;
        }
        cout << "\n";
    }
}