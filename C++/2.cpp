#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <climits>

using namespace std;

void sortByTime(vector<pair<string, int>>& visitors) {
    int n = visitors.size();
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < n - 1 - i; ++j) {
            if (visitors[j].second < visitors[j + 1].second) {
                swap(visitors[j], visitors[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

void sim() {
    cout << "Команды\nENQUEUE <время> \nDISTRIBUTE \n";

    srand(static_cast<unsigned int>(time(nullptr)));
    int windowsCount;
    cout << "Введите кол-во окон\n";
    cin >> windowsCount;

    vector<pair<int, vector<string>>> windows(windowsCount);
    for (auto& window : windows) {
        window.first = 0;
        window.second.clear();
    }

    vector<pair<string, int>> visitors;
    string cmd;
    int ticket_counter = 1;

    while (cin >> cmd) {
        if (cmd == "ENQUEUE") {
            int duration;
            cin >> duration;
            if (duration < 0) continue;

            int ticket_num = rand() % 1000;
            string ticket = "T" + to_string(ticket_num);

            visitors.emplace_back(ticket, duration);
            cout << ticket << "\n";
        }
        else if (cmd == "DISTRIBUTE") {
            break;
        }
    }

    sortByTime(visitors);
    for (auto visitor : visitors) {
        int minTime = INT_MAX;
        int bestWindow = 0;

        int index = 0;
        for (auto window : windows) {
            if (window.first < minTime){
                minTime = window.first;
                bestWindow = index;
            }
            index++;
        }

        windows[bestWindow].first += visitor.second;
        windows[bestWindow].second.push_back(visitor.first);
    }

    for (int i = 0; i < windowsCount; i++) {
        cout << "Окно " << (i + 1) << " (" << windows[i].first << " минут): ";
        if (!windows[i].second.empty()) {
            for (int j = 0; j < windows[i].second.size(); ++j) {
                if (j > 0) cout << ", ";
                cout << windows[i].second[j];
            }
        }
        cout << "\n";
    }
}
