#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
#include "Action.h"
using std::vector;

enum class PlanStatus {
    AVALIABLE,
    BUSY,
};

class Plan {
    public:
        //given
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);    
        const int getlifeQualityScore() const;
        const int getEconomyScore() const;
        const int getEnvironmentScore() const;
        void setSelectionPolicy(SelectionPolicy *selectionPolicy);
        void step();
        void printStatus();
        const vector<Facility*>& getFacilities() const;
        void addFacility(Facility* facility);
        const string toString() const;
        //
        Plan(const Plan& other, const Settlement& newSett, const vector<FacilityType> &newFacilityOptions);
        Plan(Plan&& other);
        Plan(const Plan& other);
        ~Plan();
        Plan& operator=(Plan& other)=delete;
        Plan& operator=(Plan&& other)=delete;
        bool facilityExist(Facility* facility);
        const string statusToString() const;
        void printFacilities() const;
        const int getID() const;
        const Settlement& getSettlement() const;
        const SelectionPolicy& getSelectionPolicy() const;
        Plan* clone() const;
        void clear();

    private:
        int plan_id;
        const Settlement& settlement;
        SelectionPolicy* selectionPolicy; //What happens if we change this to a reference?
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType>& facilityOptions;
        int life_quality_score, economy_score, environment_score;
        const int limit;
};