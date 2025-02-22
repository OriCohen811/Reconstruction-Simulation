#pragma once
#include "Facility.h"
#include "SelectionPolicy.h"
#include "Settlement.h"
#include "Plan.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;


class Auxiliary{
    public:
        static std::vector<std::string> parseArguments(const std::string& line);
        static int stringToInt(const std:: string& s);
        static const std::string categoryToString(const FacilityCategory& category);
        static SelectionPolicy* translatePolicy(const string &newPolicy);
        static SettlementType intToSettType(int i); //assume i in range (otherwise its return VILLAGE)
        static const FacilityCategory intToFacCategory(int i);
        static string intToStrPolic(int i);
};

