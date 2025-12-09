#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "3cmd.h"

using namespace std;

Type parseCommand(const string& cmd) {
    if (cmd == "CREATE_PLANE") return CREATE_PLANE;
    if (cmd == "PLANES_FOR_TOWN") return PLANES_FOR_TOWN;
    if (cmd == "TOWNS_FOR_PLANE") return TOWNS_FOR_PLANE;
    if (cmd == "PLANES") return PLANES;
    return invalid;
}

vector<string> split(const string& s) {
    vector<string> tokens;
    istringstream iss(s);
    string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}


void project() {
    string line;
    cout << "Команды \nCREATE_PLANE <самолет> <город> <город>...\nPLANES_FOR_TOWN <город>\nTOWNS_FOR_PLANE <самолет> \nPLANES \n";

    
    while (getline(cin, line)) {
        vector<string> tokens = split(line);

        if (tokens.empty()) {
            continue;
        }

        Type cmd = parseCommand(tokens[0]);
        switch (cmd) {
        case CREATE_PLANE:
            createPlane(tokens);
            break;
        case PLANES_FOR_TOWN:
            planesForTown(tokens);
            break;
        case TOWNS_FOR_PLANE:
            townsForPlane(tokens);
            break;
        case PLANES:
            listPlanes();
            break;
        case invalid:
            cout << "Ошибка. Нет такой команды\n";
            break;
        }
    }
    
}
