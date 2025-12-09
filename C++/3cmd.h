#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

enum Type {
    CREATE_PLANE,
    PLANES_FOR_TOWN,
    TOWNS_FOR_PLANE,
    PLANES,
    invalid
};

void createPlane(const vector<string>& args);
void planesForTown(const vector<string>& args);
void townsForPlane(const vector<string>& args);
void listPlanes();

