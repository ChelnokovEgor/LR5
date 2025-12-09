#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>

#include "1.h"
#include "2.h"
#include "3.h"
#include "4.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "Rus");
    int k;
    cin >> k;

    switch (k)
    {
    case 1:
    {
        warehouse();
        break;
    }
    case 2:
    {
        sim();
        break;
    }
    case 3:
    {
        project();
        break;
    }
    case 4:
    {
        int n;
        cin >> n;
        runSchedule(n);
        break;
    }
    default:
        break;
    }
}