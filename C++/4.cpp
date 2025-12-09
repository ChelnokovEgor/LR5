#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <cctype>

using namespace std;

map<int, vector<string>> schedule;
int currentMonth = 0;
const int daysInMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

vector<string> tokenize(const string& line) {
    vector<string> tokens;
    istringstream iss(line);
    string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

void addClass(int day, const string& subject) {
    schedule[day].push_back(subject);
}

void nextMonth() {
    int oldMonth = currentMonth;
    int newMonth = (currentMonth + 1) % 12;
    int oldMax = daysInMonth[oldMonth];
    int newMax = daysInMonth[newMonth];
    int penultimate = newMax - 1; 

    vector<string> overflow;

    for (int d = newMax + 1; d <= oldMax; d++) {
        auto it = schedule.find(d);
        if (it != schedule.end()) {
            for (auto s : it->second) {
                overflow.push_back(s);
            }
            schedule.erase(it);
        }
    }

    if (!overflow.empty()) {
        auto& penultClasses = schedule[penultimate];
        penultClasses.insert(penultClasses.end(), overflow.begin(), overflow.end());
    }

    for (auto it = schedule.begin(); it != schedule.end();) {
        if (it->first < 1 || it->first > newMax) {
            it = schedule.erase(it);
        }
        else {
            it++;
        }
    }

    currentMonth = newMonth;
}
void viewDay(int day) {
    int maxDay = daysInMonth[currentMonth];

    if (day < 1 || day > maxDay) {
        cout << "In " << day << " day We are free!\n";
        return;
    }

    auto it = schedule.find(day);
    if (it == schedule.end() || it->second.empty()) {
        cout << "In " << day << " day We are free!\n";
    }
    else {
        auto classes = it->second;
        cout << "In " << day << " day " << classes.size() << " classes in university: ";
        bool first = true;
        for (auto cls : classes) {
            if (!first) cout << ", ";
            cout << cls;
            first = false;
        }
        cout << ".\n";
    }
}


void runSchedule(int N) {
    string line;

    cout << "Команды\nCLASS <день> <предмет>\nNEXT \nVIEW <день>\n";


    for (int i = 0; i < N; ++i) {
        getline(cin, line);
        auto tokens = tokenize(line);
        if (tokens.empty()) continue;
        if (tokens[0] == "CLASS" && tokens.size() >= 3) {
            int day = stoi(tokens[1]);
            string subj = tokens[2];
            addClass(day, subj);
        }
        else if (tokens[0] == "NEXT" && tokens.size() == 1) {
            nextMonth();
        }
        else if (tokens[0] == "VIEW" && tokens.size() == 2) {
            int day = stoi(tokens[1]);
            viewDay(day);
        }
        
    }
}