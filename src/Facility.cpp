#include "Facility.h"
#include <iostream>

using namespace std;

FacilityType:: FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score): 
name(name), category(category), price(price), lifeQuality_score(lifeQuality_score), economy_score(economy_score), environment_score(environment_score){}

FacilityType:: FacilityType(const FacilityType &other): name(other.name), category(other.category), price(other.price), lifeQuality_score(other.lifeQuality_score), economy_score(other.economy_score), environment_score(other.environment_score){}

Facility:: Facility(const FacilityType& type, const string &settlementName): FacilityType(type) ,settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(type.getCost()){}

FacilityType:: ~FacilityType() = default;

FacilityType* FacilityType::clone() const{return new FacilityType(*this);}

//getters
const string& FacilityType:: getName() const{return name;}
int FacilityType:: getCost() const{return price;}
int FacilityType:: getLifeQualityScore() const{return lifeQuality_score;}
int FacilityType:: getEnvironmentScore() const{return environment_score;}
int FacilityType:: getEconomyScore() const{return economy_score;}
FacilityCategory FacilityType:: getCategory() const{return category;}
const string& Facility:: getSettlementName() const{return settlementName;}
const int Facility:: getTimeLeft() const{return timeLeft;}
const FacilityStatus &Facility::getStatus() const{return status;}

Facility:: Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score):FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score) , settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(price){}

Facility:: Facility(const Facility& other):FacilityType(other), settlementName(other.settlementName), status(other.status), timeLeft(other.timeLeft) {}

FacilityStatus Facility:: step(){
    timeLeft--;
    if(timeLeft<=0){
        setStatus(FacilityStatus::OPERATIONAL);
    }
    return status;
}

void Facility:: setStatus(FacilityStatus status){
    this->status = status;
    if(status==FacilityStatus::UNDER_CONSTRUCTIONS){
        timeLeft = price;
    }
}

const string Facility:: statusToString() const{
    if(getStatus()==FacilityStatus::OPERATIONAL){
        return "OPERATIONAL";
    }
    else{
        return "UNDER_CONSTRUCTIONS";
    }
} 

const string Facility::toString() const{return "facilityName: " + name + "\n" + "facilityStatus: " + statusToString();}

Facility* Facility::clone() const{return new Facility(*this);}
