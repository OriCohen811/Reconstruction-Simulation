#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "Auxiliary.h"
#include "Action.h"

using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;
class Plan;

class Simulation {
    public:
        //given
        Simulation(const string &configFilePath);
        void start();
        void addPlan(const Settlement &settlement, SelectionPolicy* selectionPolicy);
        void addAction(BaseAction* action);
        bool addSettlement(Settlement* settlement);
        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string& settlementName);
        Settlement &getSettlement(const string& settlementName);
        Plan &getPlan(const int planID);
        void step();
        void close();
        void open();
        //
        Simulation(const Simulation& other);
        Simulation(const Simulation&& other);
        Simulation& operator=(Simulation& other);
        Simulation& operator=(Simulation&& other);
        ~Simulation();
        void clear();
        bool crackFileLine(vector<string>& orders);
        BaseAction* crackLine(vector<string>& orders);
        bool isFacilityTypeExists(const string& facilityName);
        bool isPlanExists(int id);
        vector<BaseAction*>& getActionLog();
        Simulation* clone();

    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;
};