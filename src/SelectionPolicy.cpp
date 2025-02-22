#include "SelectionPolicy.h"
#include "Facility.h"
#include <cmath>
#include <iostream>

using namespace std;
//constructors
SelectionPolicy:: SelectionPolicy(){}
NaiveSelection::NaiveSelection(): lastSelectedIndex(-1) {}
NaiveSelection::NaiveSelection(int lastSelectedIndex): lastSelectedIndex(lastSelectedIndex){}
NaiveSelection::NaiveSelection(NaiveSelection &other) : lastSelectedIndex(other.lastSelectedIndex) {}

BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore) :LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore){}
BalancedSelection::BalancedSelection(BalancedSelection& other): LifeQualityScore(other.LifeQualityScore), EconomyScore(other.EconomyScore), EnvironmentScore(other.EnvironmentScore){}

EconomySelection::EconomySelection(): lastSelectedIndex(-1){}
EconomySelection::EconomySelection(int lastSelectedIndex): lastSelectedIndex(lastSelectedIndex){}
EconomySelection::EconomySelection(EconomySelection& other): lastSelectedIndex(other.lastSelectedIndex){}

SustainabilitySelection::SustainabilitySelection():lastSelectedIndex(0){}
SustainabilitySelection::SustainabilitySelection(SustainabilitySelection& other):lastSelectedIndex(other.lastSelectedIndex){}
SustainabilitySelection::SustainabilitySelection(int lastSelectedIndex): lastSelectedIndex(lastSelectedIndex){}

//selectFacility 
//assume there is at least one facility for each category
const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType> &facilitiesOptions){   
        int size = static_cast<int>(facilitiesOptions.size());
        lastSelectedIndex = (lastSelectedIndex + 1) % size;
        const FacilityType &choice = facilitiesOptions[lastSelectedIndex];
        return choice;
}

const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType> &facilitiesOptions){
        int size = static_cast<int>(facilitiesOptions.size());    
        int index = 0;
        int minGap = maxGap(facilitiesOptions[index]);
        if (minGap == 0){
                return facilitiesOptions[index];
        }
        for (int i = 1; i < size; i++){
                int gap = maxGap(facilitiesOptions[i]);
                if (gap < minGap){
                        minGap = gap;
                        index = i;
                        if (minGap == 0){
                                        return facilitiesOptions[index];
                                }
                }
        }
        const FacilityType& chosen = facilitiesOptions[index];
        LifeQualityScore = LifeQualityScore + chosen.getLifeQualityScore();
        EconomyScore = EconomyScore + chosen.getEconomyScore();
        EnvironmentScore = EnvironmentScore + chosen.getEnvironmentScore();
        return chosen;
}

int BalancedSelection:: maxGap(const FacilityType& facility){
        int newLQS = facility.getLifeQualityScore() + LifeQualityScore;
        int newEco = facility.getEconomyScore() + EconomyScore;
        int newEnv = facility.getEnvironmentScore() + EnvironmentScore;
        int n1 = abs(newLQS-newEco);
        int n2 = abs(newLQS-newEnv);
        int n3 = abs(newEco-newEnv);
        n1 = std::max(n1,n2);
        n1 = std::max(n1,n3);
        return n1;
}

const FacilityType& EconomySelection::selectFacility(const vector<FacilityType> &facilitiesOptions){
        int size = static_cast<int>(facilitiesOptions.size());
        for (int i = (lastSelectedIndex + 1) % size; i != lastSelectedIndex; i = (i + 1) % size){
                if (facilitiesOptions[i].getCategory() == FacilityCategory::ECONOMY){
                        lastSelectedIndex = i;
                        return facilitiesOptions[i];
                }
        }
        return facilitiesOptions[lastSelectedIndex];
}

const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType> &facilitiesOptions){
        int size = static_cast<int>(facilitiesOptions.size());
        for (int i = (lastSelectedIndex + 1) % size; i != lastSelectedIndex; i = (i + 1) % size){
                if (facilitiesOptions[i].getCategory() == FacilityCategory::ENVIRONMENT){
                        lastSelectedIndex = i;
                        return facilitiesOptions[i];
                }
        }
        return facilitiesOptions[lastSelectedIndex];
}

//strings
const string NaiveSelection::toString() const{return "nve";}
const string BalancedSelection::toString() const{return "bal";}
const string EconomySelection::toString() const{return "eco";}
const string SustainabilitySelection::toString() const{return "env";}

//copy
NaiveSelection* NaiveSelection::clone() const{return (new NaiveSelection(lastSelectedIndex));}
BalancedSelection* BalancedSelection::clone() const{return (new BalancedSelection(LifeQualityScore, EconomyScore, EnvironmentScore));}
EconomySelection* EconomySelection::clone() const{return new EconomySelection(lastSelectedIndex);}
SustainabilitySelection* SustainabilitySelection::clone() const{return new SustainabilitySelection(lastSelectedIndex);}

//operators
NaiveSelection& NaiveSelection::operator=(const NaiveSelection& other) {       
        if(this!=&other){
                lastSelectedIndex=other.lastSelectedIndex;
        }
        return  *this;
}

BalancedSelection& BalancedSelection::operator=(const BalancedSelection& other) {
        if(this!=&other){
                EconomyScore = other.EconomyScore;
                EnvironmentScore = other.EnvironmentScore;
                LifeQualityScore = other.LifeQualityScore;
        }      
        return  *this;
}

EconomySelection& EconomySelection::operator=(const EconomySelection& other)  {
        if(this!=&other){
                lastSelectedIndex=other.lastSelectedIndex;
        }
        return  *this;
}

SustainabilitySelection& SustainabilitySelection::operator=(const SustainabilitySelection& other) {
        if(this!=&other){
                lastSelectedIndex=other.lastSelectedIndex;
        }
        return  *this;
}

//getters
PolicyType NaiveSelection:: getPlcType() const {return PolicyType::NVE;}
PolicyType BalancedSelection:: getPlcType() const {return PolicyType::BAL;}
PolicyType EconomySelection:: getPlcType() const {return PolicyType::ECO;}
PolicyType SustainabilitySelection:: getPlcType() const {return PolicyType::ENV;}

