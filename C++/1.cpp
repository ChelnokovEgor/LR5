#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cctype>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <clocale>

using namespace std;

int ZONES_COUNT = 2;
int RACKS_PER_ZONE = 6;
int SECTIONS_PER_RACK = 4;
int SHELVES_PER_SECTION = 4;
int SHELVES_CAPACITY = 10;
int ZONE_CAPACITY = RACKS_PER_ZONE * SECTIONS_PER_RACK * SHELVES_PER_SECTION * SHELVES_CAPACITY;
int TOTAL_CAPACITY = ZONE_CAPACITY * ZONES_COUNT;


string makeAddress(char zone, int rack, int section, int shelf) {
    return zone + to_string(rack) + to_string(section) + to_string(shelf);
}


bool isValidAddress(const string& addr) {
    if (addr.size() != 4)
        return false;
    char z = addr[0];
    if (z != 'A' && z != 'B')
        return false;
    for (int i = 1; i <= 3; ++i) {
        if (!isdigit(addr[i]))
            return false;
    }
    int r = addr[1] - '0';
    int s = addr[2] - '0';
    int sh = addr[3] - '0';
    return (r >= 1 && r <= RACKS_PER_ZONE) && (s >= 1 && s <= SECTIONS_PER_RACK) && (sh >= 1 && sh <= SHELVES_PER_SECTION);
}


void initWarehouse(map<string, string>& productMap, map<string, int>& countMap) {
    const string zones = "AB";
    for (char z : zones) {
        for (int rack = 1; rack <= RACKS_PER_ZONE; ++rack) {
            for (int sec = 1; sec <= SECTIONS_PER_RACK; ++sec) {
                for (int shelf = 1; shelf <= SHELVES_PER_SECTION; ++shelf) {
                    string addr = makeAddress(z, rack, sec, shelf);
                    productMap[addr] = "";
                    countMap[addr] = 0;
                }
            }
        }
    }
}


vector<string> splitCommand(const string& s) {
    istringstream iss(s);
    vector<string> tokens;
    string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}


void warehouseAdd(map<string, string>& productMap, map<string, int>& countMap, const vector<string>& tokens) {
    if (tokens.size() < 4) {
        cout << "Ошибка: Недостаточно аргументов. Формат: ADD <товар> <кол-во> <адрес>\n";
        return;
    }
    string product = tokens[1];
    string addr = tokens[3];
    int count;
    try {
        count = stoi(tokens[2]);
    }
    catch (...){
        cout << "Ошибка: Некорректное количество.\n";
        return;
    }
    if (count <= 0) {
        cout << "Ошибка: Количество должно быть > 0.\n";
        return;
    }
    if (!isValidAddress(addr)) {
        cout << "Ошибка: Некорректный адрес '" << addr << "'. Правила: <AB> <1-6> <1-4> <1-4>\n";
        return;
    }


    string currentProduct = productMap[addr];
    int currentCount = countMap[addr];

    if (!currentProduct.empty() && currentProduct != product) {
        cout << "Ошибка: В ячейке " << addr << " уже лежит '" << currentProduct << "'. Нельзя положить '" << product << "'.\n";
        return;
    }

    if (currentCount + count > SHELVES_CAPACITY) {
        int free = SHELVES_CAPACITY - currentCount;
        cout << "Ошибка: В ячейке '" << addr << "' только " << free << "свободных мест.\n";
        return;
    }

    productMap[addr] = product;
    countMap[addr] = count;
    cout << "Добавлено " << count << " шт. '" << product << "' в " << addr << ".\n";
}


void warehousePop(map<string, string>& productMap, map<string, int>& countMap, const vector<string>& tokens) {
    if (tokens.size() < 4) {
        cout << "Ошибка: Недостаточно аргументов. Формат: REMOVE <товар> <кол-во> <адрес>\n";
        return;
    }

    string product = tokens[1];
    string addr = tokens[3];
    int count;
    try {
        count = stoi(tokens[2]);
    }
    catch (...) {
        cout << "Ошибка: Некорректное количество.\n";
        return;
    }
    if (count <= 0) {
        cout << "Ошибка: Количество должно быть > 0.\n";
        return;
    }
    if (!isValidAddress(addr)) {
        cout << "Ошибка: Некорректный адрес '" << addr << "'. Правила: <AB> <1-6> <1-4> <1-4>\n";
        return;
    }

    string currentProduct = productMap[addr];
    int currentCount = countMap[addr];

    if (currentProduct != product) {
        cout << "Ошибка: В ячейке " << addr << " лежит '" << currentProduct << "', а не '" << product << "'.\n";
        return;
    }
    if (count > currentCount) {
        cout << "Ошибка: В ячейке " << addr << " только " << currentCount << " шт.\n";
        return;
    }

    currentCount -= count;
    if (currentCount == 0) {
        currentProduct = "";
    }
    cout << "Удалено " << count << " шт. '" << product << "' из " << addr << ".\n";
}


void warehouseInfo(const map<string, string>& productMap, const map<string, int>& countMap) {
    int totalUsed = 0;
    int zoneAUsed = 0;
    int zoneBUsed = 0;
    vector<string> occupied, empty;

    for (auto entry : productMap) {
        const string addr = entry.first;
        const string product = entry.second;
        int count = countMap.at(addr);

        /*
        cout << addr << endl;
        cout << product << endl;
        cout << count << endl;
        cout << endl;
        */

        totalUsed += count;
        if (addr[0] == 'A') zoneAUsed += count;
        else if (addr[0] == 'B') zoneBUsed += count;

        if (count > 0) {
            occupied.push_back(addr);
        }
        else {
            empty.push_back(addr);
        }
    }

    cout << "Общая загрузка склада: " << fixed << setprecision(2) << 100 * totalUsed / TOTAL_CAPACITY << "%" << endl;
    cout << "Загрузка зоны A: " << fixed << setprecision(2) << 100 * zoneAUsed / ZONE_CAPACITY << "%" << endl;
    cout << "Загрузка зоны B: " << fixed << setprecision(2) << 100 * zoneBUsed / ZONE_CAPACITY << "%" << endl;

    cout << "Содержание занятых ячеек:" << endl;
    for (const string& addr : occupied) {
        cout << addr << ": " << productMap.at(addr) << " (" << countMap.at(addr) << ")" << endl;
    }

    cout << "Пустые ячейки:" << endl;
    for (int i = 0; i < empty.size(); ++i) {
        cout << empty[i];
        if (i < empty.size() - 1) cout << ", ";
    }
}


void warehouse() {
    map<string, string> productMap;
    map<string, int> countMap;

    initWarehouse(productMap, countMap);
    string line;

    cout << "Команды\nADD <товар> <кол-во> <адрес>\nREMOVE <товар> <кол-во> <адрес>\nINFO\nEXIT\n";

    while (getline(cin, line)) {
        auto tokens = splitCommand(line);
        if (tokens.empty()) continue;
        string cmd = tokens[0];

        if (cmd == "ADD") {
            warehouseAdd(productMap, countMap, tokens);
        }
        else if (cmd == "REMOVE") {
            warehousePop(productMap, countMap, tokens);
        }
        else if (cmd == "INFO") {
            warehouseInfo(productMap, countMap);
        }
        else if (cmd == "EXIT") {
            break;
        }
        else {
            cout << "Неизвестная команда: " << cmd << endl;
        }
    }
}