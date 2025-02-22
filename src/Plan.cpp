#include "Plan.h"
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
#include <iostream>

using namespace std;


Plan:: Plan(const int planId, const Settlement &settlement, SelectionPolicy* selectionPolicy, const vector<FacilityType> &facilityOptions): plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy), status(PlanStatus:: AVALIABLE),facilities(), underConstruction(),facilityOptions(facilityOptions), life_quality_score(0), economy_score(0), environment_score(0), limit(1+static_cast<int>(settlement.getType())){}

Plan:: Plan(const Plan& other, const Settlement& newSett, const vector<FacilityType> &newFacilityOptions):plan_id(other.plan_id), settlement(newSett), selectionPolicy(other.selectionPolicy->clone()), status(other.status), facilities(), underConstruction(),facilityOptions(newFacilityOptions),life_quality_score(other.life_quality_score), economy_score(other.economy_score), environment_score(other.environment_score), limit(other.limit){
    for(Facility* fac: other.facilities){
        Facility* copy = new Facility(*fac);
        addFacility(copy);
    }
    for(Facility* fac : other.underConstruction){
        Facility* copy = new Facility(*fac);
        underConstruction.push_back(copy);
    }
}

Plan:: Plan(const Plan& other): plan_id(other.plan_id), settlement(other.settlement), selectionPolicy(other.selectionPolicy->clone()), status(other.status), facilities(), underConstruction(),facilityOptions(other.facilityOptions),life_quality_score(other.life_quality_score), economy_score(other.economy_score), environment_score(other.environment_score), limit(other.limit){
    for(Facility* fac: other.facilities){
        Facility* copy = new Facility(*fac);
        addFacility(copy);
    }
    for(Facility* fac : other.underConstruction){
        Facility* copy = new Facility(*fac);
        underConstruction.push_back(copy);
    }

}

Plan:: Plan(Plan&& other): plan_id(other.plan_id), settlement(other.settlement), selectionPolicy(other.selectionPolicy), status(other.status), facilities(), underConstruction(), facilityOptions(other.facilityOptions), life_quality_score(other.life_quality_score), economy_score(other.economy_score), environment_score(other.environment_score), limit(other.limit){
    for (Facility *fac : other.facilities){
        facilities.push_back(fac);
        fac = nullptr;
    }
    other.facilities.clear();

    for (Facility *fac : other.underConstruction){
        facilities.push_back(fac);
        fac = nullptr;
    }
    other.underConstruction.clear();
    other.selectionPolicy = nullptr;
}

Plan:: ~Plan(){
    clear();
}

void Plan:: clear(){
    if(selectionPolicy!=nullptr){
        delete selectionPolicy;
    }
    selectionPolicy = nullptr;
    for (Facility *fac : facilities){
        if(fac!=nullptr){
            delete fac;
        }
        fac = nullptr;
    }
    facilities.clear();
        
    for (Facility* fac : underConstruction){
        if(fac!=nullptr){
            delete fac;
        }
        fac = nullptr;
    }
    underConstruction.clear();
}
      
void Plan:: setSelectionPolicy(SelectionPolicy* selectionPolicy){ //assumed is valid
    delete this->selectionPolicy;
    this->selectionPolicy = selectionPolicy;
}
        
void Plan:: step(){
    //assume there is always facility to build
    int size = static_cast<int> (underConstruction.size());
    for(int i=size; i<limit && status==PlanStatus::AVALIABLE;i++){
        const FacilityType& type = selectionPolicy->selectFacility(facilityOptions);
        Facility* newFac = new Facility(type, settlement.getName());
        newFac->setStatus(FacilityStatus::UNDER_CONSTRUCTIONS);
        underConstruction.push_back(newFac);
    }
    size = static_cast<int>(underConstruction.size());
    for(int i=size-1; i>=0; i--){
        Facility* fac = underConstruction[i];
        FacilityStatus facStatus = fac->step();
        if(facStatus==FacilityStatus::OPERATIONAL){
            underConstruction[i] = nullptr;
            addFacility(fac);
            life_quality_score = life_quality_score + fac->getLifeQualityScore();
            economy_score = economy_score + fac->getEconomyScore();
            environment_score = environment_score + fac->getEnvironmentScore();
            underConstruction.erase(underConstruction.begin() + i);
        }
    }
    size = static_cast<int>(underConstruction.size());
    if (size == limit){
        status = PlanStatus::BUSY;
    }
    else{
        status = PlanStatus::AVALIABLE;
    }
}
                    
void Plan:: addFacility(Facility* facility){
    facilities.push_back(facility);
}

bool Plan:: facilityExist(Facility* facility){
    for(Facility* fac : facilities){
        if((*fac).getName() == (*facility).getName()){
            return true;
        }
    }
    return false;
}

const string Plan:: statusToString() const{
    if(status == PlanStatus::AVALIABLE){
        return "AVALIABLE";
    }
    else{
        return "BUSY";
    }
}

void Plan:: printFacilities() const {
    for(Facility* fac_p: underConstruction){
        cout<< fac_p->toString() <<endl;
    }
    for(Facility* fac_p: facilities){
        cout<< fac_p->toString() <<endl;
    }
}
        
const string Plan:: toString() const{
    const string output =   "planID: " + to_string(plan_id) + "\n" +
                            settlement.toString() +"\n"+
                            "LifeQualityScore: " + to_string(getlifeQualityScore()) +"\n"+
                            "EconomyScore: "+ to_string(getEconomyScore()) + "\n" +
                            "EnvrionmentScore: " + to_string(getEnvironmentScore()) +"\n";
    return output;
}

void Plan:: printStatus(){
    cout << "planID: " + to_string(plan_id) + "\n" +
                            settlement.toString() +"\n"+
                            "planStatus: " + statusToString() +"\n"+
                            "selectionPolicy: "  + (*selectionPolicy).toString() +"\n"+
                            "LifeQualityScore: " + to_string(getlifeQualityScore()) +"\n"+
                            "EconomyScore: "+ to_string(getEconomyScore()) + "\n" +
                            "EnvrionmentScore: " + to_string(getEnvironmentScore()) << endl;
    printFacilities();
}

//getters
const vector<Facility*>& Plan:: getFacilities() const{return facilities;}
const int Plan:: getlifeQualityScore() const{return life_quality_score;}
const int Plan:: getEconomyScore() const{return economy_score;}       
const int Plan:: getEnvironmentScore() const{return environment_score;}
const Settlement& Plan::getSettlement() const{return settlement;}
const SelectionPolicy& Plan::getSelectionPolicy() const{return *selectionPolicy;}
const int Plan:: getID() const{return plan_id;}

Plan* Plan:: clone() const {
    const Settlement* sett = settlement.clone();
    const Settlement& sett_r = *sett;
    SelectionPolicy* pol = selectionPolicy->clone();
    Plan* newPlan = new Plan(plan_id, sett_r, pol, facilityOptions);
    newPlan->status = status;
    for(Facility* fac: facilities){
        Facility* copy = new Facility(*fac);
        (newPlan->facilities).push_back(copy);
    }
    for(Facility* fac: underConstruction){
        Facility* copy = new Facility(*fac);
        (newPlan->underConstruction).push_back(copy);
    }
    newPlan->life_quality_score = life_quality_score;
    newPlan->economy_score = economy_score;
    newPlan->environment_score = environment_score;

    return newPlan;
}

