#include "Simulation.h"
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
#include "Auxiliary.h"
#include "Action.h"
#include <bits/stdc++.h>
#include <iostream>

using namespace std;

//constructors
Simulation:: Simulation(const string &configFilePath): isRunning(false), planCounter(0), actionsLog(), plans(), settlements(), facilitiesOptions(){
    ifstream f(configFilePath);
    if (!f.is_open()) {
        cout << "Error opening the file!"<<endl;
    }
    else{
        string s;
        int l = 1;
        while (getline(f, s)){
            vector<string> orders(Auxiliary::parseArguments(s));
            int size = static_cast<int>(orders.size());
            if (size >= 3 && orders[0]!="#"){
                bool done = crackFileLine(orders);
                if (!done){
                    cout << "Line " + to_string(l) + ": invalid line " << endl;
                }
            }
            else if(size>0 && orders[0]!="#"){
                cout << "Line " + to_string(l) + ": invalid line" << endl;
            }
            orders.clear();
            l++;
        }
    }   
}

Simulation:: Simulation(const Simulation& other):isRunning(other.isRunning), planCounter(other.planCounter), actionsLog(), plans(), settlements(), facilitiesOptions(){
    for (Settlement* sett : other.settlements) {
        settlements.push_back(sett->clone());
    }

    for(const FacilityType& fac: other.facilitiesOptions){
        facilitiesOptions.push_back(fac);
    }
    
    int size = static_cast<int>(other.plans.size());
    for(int i=0; i<size; i++){
        const Plan& p = other.plans[i];
        plans.push_back(Plan(p, getSettlement(p.getSettlement().getName()), facilitiesOptions));
    }

    for (BaseAction* act : other.actionsLog) {
        actionsLog.push_back(act->clone());
    }
    
}

Simulation::Simulation(const Simulation&& other):isRunning(other.isRunning), planCounter(other.planCounter), actionsLog(), plans(), settlements(), facilitiesOptions(){
    for (Settlement* sett : other.settlements) {
        settlements.push_back(sett);
        sett = nullptr;
    }

    for(const FacilityType& fac: other.facilitiesOptions){
        facilitiesOptions.push_back(fac);
    }
    
    int size = static_cast<int>(other.plans.size());
    for(int i=0; i<size; i++){
        const Plan& p = other.plans[i];
        plans.push_back(Plan(p, getSettlement(p.getSettlement().getName()), facilitiesOptions));
    }

    for (BaseAction* act : other.actionsLog) {
        actionsLog.push_back(act);
        act = nullptr;
    }
    
}

//destructor
Simulation:: ~Simulation(){
    clear();
}

void Simulation:: clear(){
    for (BaseAction* action : actionsLog) {
        if(action!=nullptr){
            delete action; 
        }
        action = nullptr;
    }
    actionsLog.clear();
    for (Settlement* settlement : settlements) {
        if(settlement!=nullptr){
            delete settlement;  
        }
        settlement=nullptr;
    }
    settlements.clear();

    plans.clear();

    facilitiesOptions.clear();
}

//operators
Simulation& Simulation::operator=(Simulation& other) {
    if (this != &other) {
        clear();
        isRunning = false; //need to use open()
        planCounter = other.planCounter;

        for (Settlement* sett : other.settlements) {
            settlements.push_back(sett->clone());
        }

        for(const FacilityType& fac: other.facilitiesOptions){
            facilitiesOptions.push_back(fac);
        }

        int size = static_cast<int>(other.plans.size());
        for(int i=0; i<size; i++){
            const Plan& p = other.plans[i];
            plans.push_back(Plan(p, getSettlement(p.getSettlement().getName()), facilitiesOptions));
        }

        for (BaseAction* act : other.actionsLog) {
            actionsLog.push_back(act->clone());
        }
    }
    return *this;
}

Simulation* Simulation:: clone(){
    return new Simulation(*this);
}

Simulation& Simulation::operator=(Simulation&& other){
    if (this != &other) {
        clear();
        isRunning = false; //need to use open()
        planCounter = other.planCounter;

        for (Settlement* sett : other.settlements) {
        settlements.push_back(sett);
        sett = nullptr;
    }

    for(const FacilityType& fac: other.facilitiesOptions){
        facilitiesOptions.push_back(fac);
    }
    
    int size = static_cast<int>(other.plans.size());
    for(int i=0; i<size; i++){
        const Plan& p = other.plans[i];
        plans.push_back(Plan(p, getSettlement(p.getSettlement().getName()), facilitiesOptions));
    }

    for (BaseAction* act : other.actionsLog) {
        actionsLog.push_back(act);
        act = nullptr;
    }
    }
    return *this;
}

//decoders
bool Simulation::crackFileLine(vector<string>& orders){
    const string arg1 = orders[0]; //command's name
    const string arg2 = orders[1]; 
    const string arg3 = orders[2];
    if (arg1 == "settlement"){
        if (orders.size() == 3){
            int type3  = Auxiliary::stringToInt(arg3);
            if (!(type3 < 0 || type3 > 2)){
                SettlementType type = Auxiliary::intToSettType(type3);
                Settlement* newSett = new Settlement(arg2, type);
                bool done = addSettlement(newSett);
                if(!done){
                    delete newSett;
                }
                newSett = nullptr;
                return done;
            }
        }
    }
    else if (arg1 == "facility"){
        if (orders.size() == 7){
            string arg4 = orders[3],  arg5 = orders[4], arg6 = orders[5], arg7 = orders[6];
            int category = Auxiliary::stringToInt(arg3), price = Auxiliary::stringToInt(arg4), lifeq = Auxiliary::stringToInt(arg5), eco = Auxiliary::stringToInt(arg6), env = Auxiliary::stringToInt(arg7);
            if (!(category < 0 || category > 2 || price == -1 || lifeq == -1 || eco == -1 || env == -1)){
                FacilityCategory facate = Auxiliary::intToFacCategory(category);
                bool done = addFacility(FacilityType(arg2, facate, price, lifeq, eco, env));
                return done;
            }
            else{
                return false;
            }
        }
    }
    else if (arg1 == "plan"){
        if (orders.size() == 3){
            if(isSettlementExists(arg2)){
                Settlement& sett_p = getSettlement(arg2);
                SelectionPolicy* policy = Auxiliary::translatePolicy(arg3);
                if(policy!=nullptr){
                    addPlan(sett_p, policy);
                    return true;
                }
                else{
                    return false;
                }
            }
        }
    }
    return false;
}

BaseAction* Simulation::crackLine(vector<string>& orders){
    BaseAction* action = nullptr; 
    const string arg1 = orders[0]; //command's name
    int size = static_cast<int>(orders.size());
    if (size == 1){
        if (arg1 == "log"){action = new PrintActionsLog(),action->act(*this);}
        else if (arg1 == "close"){action = new Close(),action->act(*this);}
        else if (arg1 == "backup"){action = new BackupSimulation(),action->act(*this);}
        else if (arg1 == "restore"){action = new RestoreSimulation(),action->act(*this);}
    }
    else if (size == 2){
        const string arg2 = orders[1];
        if (arg1 == "step"){
            int amount = Auxiliary::stringToInt(arg2);
            if (amount > 0){
                action = new SimulateStep(amount);
                action->act(*this);
            }
        }
        else if (arg1 == "planStatus"){
            int plan = Auxiliary::stringToInt(arg2);
            action = new PrintPlanStatus(plan);
            action->act(*this);
        }
    }
    else if(size>=3){
        const string arg2 = orders[1];
        const string arg3 = orders[2];
        if (arg1 == "settlement"){
            int type3  = Auxiliary::stringToInt(arg3);
            if ((type3 >= 0 && type3 <= 2)){ 
                SettlementType type = Auxiliary::intToSettType(type3);
                action = new AddSettlement(arg2, type);
                action->act(*this);
            }
        }
        else if (arg1 == "facility"){
            if (size == 7){
                string arg4 = orders[3];
                string arg5 = orders[4];
                string arg6 = orders[5];
                string arg7 = orders[6];
                int category = Auxiliary::stringToInt(arg3);
                int price = Auxiliary::stringToInt(arg4);
                int lifeq = Auxiliary::stringToInt(arg5);
                int eco = Auxiliary::stringToInt(arg6);
                int env = Auxiliary::stringToInt(arg7);
                if (!(category < 0 || category > 2 || price == -1 || lifeq == -1 || eco == -1 || env == -1)){
                    FacilityCategory facate = Auxiliary::intToFacCategory(category);
                    action = new AddFacility(arg2, facate,price, lifeq, eco, env);
                    action->act(*this);
                }
            }
        }
        else if (arg1 == "plan"){
            action = new AddPlan(arg2, arg3);
            action->act(*this);
            
        }
        else if (arg1 == "changePolicy"){
            int plan = Auxiliary::stringToInt(arg2);
            const string& policy = arg3;
            action = new ChangePlanPolicy(plan, policy);
            action->act(*this);    
        }
    }
    return action;
}

//addition functions
void Simulation::addPlan(const Settlement& settlement, SelectionPolicy* selectionPolicy){
    plans.push_back(Plan(planCounter, settlement, selectionPolicy, facilitiesOptions));
    planCounter++;
}

void Simulation::addAction(BaseAction* action){
    if (action != nullptr){
        actionsLog.push_back(action);
    }
    else{
        delete action;
    }
}

bool Simulation::addSettlement(Settlement* settlement){
    if (isSettlementExists(settlement->getName())){
        return false;
    }
    else{
        settlements.push_back(settlement);
        return true;
    }
}

bool Simulation:: addFacility(FacilityType facility){
    if(!isFacilityTypeExists(facility.getName())){
        facilitiesOptions.push_back(facility);
        return true;
    }
    else{
        return false;
    }
}

//boolean functions
bool Simulation:: isFacilityTypeExists(const string& facilityName){
    for(FacilityType fac: facilitiesOptions){
        if(fac.getName() == facilityName){
            return true;
        }
    }
    return false;
}

bool Simulation:: isSettlementExists(const string &settlementName){
    for(Settlement* sett: settlements){
        if((*sett).getName() == settlementName){
            return true;
        }
    }
    return false;
}

bool Simulation:: isPlanExists(int id){
    if(id<0 || id>=planCounter){
        return false;
    }
    else{
        return true;
    }
}

//getters
Settlement& Simulation:: getSettlement(const string &settlementName){
    //assume the settlement exists (checking performed before)
    for(Settlement* sett: settlements){
        if((*sett).getName() == settlementName){
            return *sett;
        }
    }
    return *settlements[0]; //doesnt supposed to reach here
}

Plan& Simulation :: getPlan(const int planID){
    //assume planID valid (checking performed before)
    return plans[planID];
}

vector<BaseAction*>& Simulation:: getActionLog(){
    vector<BaseAction*>& logs = actionsLog;
    return logs;
}

void Simulation::start(){
    open();
    cout << "The simulation has started" << endl;
    while (isRunning){
        string order;
        getline(cin, order);
        vector<string> ords = Auxiliary::parseArguments(order);
        if(!ords.empty()){
            addAction(crackLine(ords));
        }
    }
}

void Simulation:: step(){
    for(Plan& plan: plans){
            plan.step();
    }
}

void Simulation:: close(){
    isRunning = false;
    for(Plan p:plans){
        cout<< p.toString() << endl;
    }
}

void Simulation:: open(){
    isRunning = true;
}



