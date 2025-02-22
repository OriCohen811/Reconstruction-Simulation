#include "Action.h"
#include "Simulation.h"
#include "SelectionPolicy.h"
#include "Auxiliary.h"
#include <iostream>

using namespace std;

BaseAction:: BaseAction():errorMsg(""), status(ActionStatus::ERROR){}

ActionStatus BaseAction:: getStatus() const{
    return status;
}

void BaseAction:: complete(){
    status = ActionStatus::COMPLETED;
}

void BaseAction:: error(string errorMsg){
    this->errorMsg = "Error: " + errorMsg;
    this->status = ActionStatus::ERROR;
}

const string BaseAction:: statusToString() const{
    if(status==ActionStatus::COMPLETED){
        return "COMPLETED";
    }
    else{
        return "ERROR";
    }
}

const string& BaseAction::getErrorMsg() const{
    return errorMsg;
}

SimulateStep:: SimulateStep(const int numOfSteps): numOfSteps(numOfSteps){}

void SimulateStep:: act(Simulation& simulation) {
    for(int i=0; i<numOfSteps; i++){
        simulation.step();
    }
    complete();
}

const string SimulateStep:: toString() const {
    return "SimulateStep " + to_string(numOfSteps) + " " + statusToString();
}

SimulateStep* SimulateStep:: clone() const {
    SimulateStep* copy = new SimulateStep(numOfSteps);
    if(getStatus()==ActionStatus::ERROR){
        copy->error(getErrorMsg());
    }
    else{
        copy->complete();
    }
    return copy;
}

AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy):settlementName(settlementName), selectionPolicy(selectionPolicy){}

void AddPlan:: act(Simulation &simulation) {
    SelectionPolicy* policy = Auxiliary:: translatePolicy(selectionPolicy);
    if(policy==nullptr || !simulation.isSettlementExists(settlementName)){
        error("Cannot create this plan");
        cout << getErrorMsg() << endl;
        delete policy;
        return;
    }
    Settlement& sett_p = simulation.getSettlement((*this).settlementName);
    simulation.addPlan(sett_p, policy);
    complete();
    
    policy = nullptr;
}

const string AddPlan:: toString() const {
    return "Plan " + settlementName + " " + selectionPolicy + " " + statusToString(); 
}

AddPlan* AddPlan:: clone() const {
    AddPlan* copy = new AddPlan(settlementName, selectionPolicy);
    if(getStatus()==ActionStatus::ERROR){
        copy->error(getErrorMsg());
    }
    else{
        copy->complete();
    }
    return copy;
}

AddSettlement:: AddSettlement(const string &settlementName, SettlementType settlementType): settlementName(settlementName) ,settlementType(settlementType){}

void AddSettlement:: act(Simulation &simulation) {
    SettlementType type = settlementType;
    Settlement* newSett = new Settlement(settlementName, type);
    bool done = simulation.addSettlement(newSett);
    if(!done){
        error("Settlement already exists");
        cout << getErrorMsg() << endl;
        delete newSett;
    }
    else{
        complete();
    }
    newSett = nullptr;
}

AddSettlement* AddSettlement:: clone() const {
    AddSettlement* copy = new AddSettlement(settlementName, settlementType);
    if(getStatus()==ActionStatus::ERROR){
        copy->error(getErrorMsg());
    }
    else{
        copy->complete();
    }
    return copy;
}

const string AddSettlement:: toString() const {
    return "Settlement " + settlementName + " " + settlementTypeToString() + " " + statusToString();
}

const string AddSettlement::settlementTypeToString() const {
    if(settlementType== SettlementType::CITY){
        return "CITY";
    }
    else if(settlementType== SettlementType::METROPOLIS){
        return "METROPOLIS";
    }
    else{
        return "VILLAGE";
    }
}

AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore): facilityName(facilityName), facilityCategory(facilityCategory), price(price), lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore){}

void AddFacility::act(Simulation &simulation) {
    FacilityType* fac = new FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
    if(simulation.addFacility(*fac)){
        complete();
    }
    else{
        error("Facility already exists");
        cout<< getErrorMsg() << endl;
    }
    delete fac;
}

AddFacility* AddFacility::clone() const {
    AddFacility* copy = new AddFacility(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
    if(getStatus()==ActionStatus::ERROR){
        copy->error(getErrorMsg());
    }
    else{
        copy->complete();
    }
    return copy;
}

const string AddFacility::toString() const {
    return "Facility " + facilityName + " " + Auxiliary::categoryToString(facilityCategory) + " " + to_string(price) + " " + to_string(lifeQualityScore) + " " + to_string(economyScore) + " " + to_string(environmentScore) + " " + statusToString();
}

PrintPlanStatus::PrintPlanStatus(int planId):planId(planId){}

void PrintPlanStatus::act(Simulation &simulation) {
    if(!simulation.isPlanExists(planId)){
        error("Plan doesn’t exist");
        cout<< getErrorMsg() << endl;
    }
    else{
        Plan& p = simulation.getPlan(planId);
        p.printStatus();
        complete();
    }
}

PrintPlanStatus* PrintPlanStatus::clone() const {
    PrintPlanStatus* copy = new PrintPlanStatus(planId);
    if(getStatus()==ActionStatus::ERROR){
        copy->error(getErrorMsg());
    }
    else{
        copy->complete();
    }
    return copy;
}

const string PrintPlanStatus::toString() const {
    return "PrintPlanStatus " + to_string(planId) + " " + statusToString();
}

ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy): planId(planId), newPolicy(newPolicy){}

void ChangePlanPolicy::act(Simulation &simulation) {
    if(!simulation.isPlanExists(planId)){
        error("Cannot change selection policy");
        cout<< getErrorMsg() <<endl;
    }
    else{
        Plan& p = simulation.getPlan(planId);
        SelectionPolicy* policy = translatePolicy(newPolicy,p);
        if(policy==nullptr || (p.getSelectionPolicy()).toString() == policy->toString()){
            error("Cannot change selection policy");
            cout << getErrorMsg() << endl;
            delete policy;
        }
        else{
            p.setSelectionPolicy(policy);
            complete();
            policy = nullptr;
        }
    }
}

SelectionPolicy* ChangePlanPolicy:: translatePolicy(const string &newPolicy, const Plan& plan){
    SelectionPolicy* p = nullptr;
    if(newPolicy=="nve"){
        p = new NaiveSelection(); 
    }
    else if(newPolicy=="bal"){
        p = new BalancedSelection(plan.getlifeQualityScore(), plan.getEconomyScore(), plan.getEnvironmentScore());
    }
    else if(newPolicy=="eco"){
        p = new EconomySelection();
        
    }
    else if(newPolicy=="env"){
        p = new SustainabilitySelection();
    }
    return p;
}

ChangePlanPolicy* ChangePlanPolicy::clone() const {
    ChangePlanPolicy* copy =  new ChangePlanPolicy(planId, newPolicy);
    if(getStatus()==ActionStatus::ERROR){
        copy->error(getErrorMsg());
    }
    else{
        copy->complete();
    }
    return copy;
}

const string ChangePlanPolicy::toString() const {
    return "ChangePlanPolicy " + to_string(planId) + " " + newPolicy + " " + statusToString();
}

PrintActionsLog::PrintActionsLog(){}

void PrintActionsLog ::act(Simulation &simulation) {
    vector<BaseAction*>& logs = simulation.getActionLog();
    for(BaseAction* action: logs){
        cout<< action->toString() << endl;
    }
    complete();
}

PrintActionsLog* PrintActionsLog::clone() const {
    PrintActionsLog* copy = new PrintActionsLog();
    if(getStatus()==ActionStatus::ERROR){
        copy->error(getErrorMsg());
    }
    else{
        copy->complete();
    }
    return copy;
}

const string PrintActionsLog::toString() const {
    return "PrintActionsLog " + statusToString();
}

Close:: Close(){}

void Close:: act(Simulation &simulation) {
    complete();
    simulation.close();
}

Close* Close:: clone() const {
    Close* copy = new Close();
    copy->complete();
    return copy;
}

const string Close:: toString() const {
    return "Close " + statusToString();
}

BackupSimulation:: BackupSimulation(){}

void BackupSimulation:: act(Simulation &simulation){
    extern Simulation* backup;
    if(backup!=nullptr){
        delete backup;
    }
    backup = simulation.clone();
    complete();
}

BackupSimulation* BackupSimulation::clone() const{
    BackupSimulation* copy = new BackupSimulation();
    if(getStatus()==ActionStatus::ERROR){
        copy->error(getErrorMsg());
    }
    else{
        copy->complete();
    }
    return copy;
}

const string BackupSimulation::toString() const {
    return "BackupSimulation " + statusToString();
}

RestoreSimulation:: RestoreSimulation(){}

void RestoreSimulation:: act(Simulation &simulation){
    extern Simulation* backup;
    if(backup==nullptr){
        error("No backup available");
        cout << getErrorMsg() << endl;
    }
    else{
        simulation.clear();
        simulation = *backup;
        simulation.open();
        complete();
    }
}

RestoreSimulation* RestoreSimulation:: clone() const {
    RestoreSimulation* copy = new RestoreSimulation();
    if(getStatus()==ActionStatus::ERROR){
        copy->error(getErrorMsg());
    }
    else{
        copy->complete();
    }
    return copy;
}

const string RestoreSimulation::toString() const {
    return "RestoreSimulation " + statusToString();
}