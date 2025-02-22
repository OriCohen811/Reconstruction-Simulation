#include "Auxiliary.h"

using namespace std;
/*
This is a 'static' method that receives a string(line) and returns a vector of the string's arguments.

For example:
parseArguments("settlement KfarSPL 0") will return vector with ["settlement", "KfarSPL", "0"]

To execute this method, use Auxiliary::parseArguments(line)
*/
std::vector<std::string> Auxiliary::parseArguments(const std::string& line) {
    std::vector<std::string> arguments;
    std::istringstream stream(line);
    std::string argument;

    while (stream >> argument) {
        arguments.push_back(argument);
    }

    return arguments;
}

const std::string Auxiliary::categoryToString(const FacilityCategory& category){
    std::string s = "bal";
    if(category==FacilityCategory::ECONOMY){
        s =  "eco";
    }
    else if(category==FacilityCategory::ENVIRONMENT){
        s=  "env";
    }
    return s;
}

int Auxiliary:: stringToInt(const std:: string& s){
    int output = 0;
    if(s.length()==0){  
        return -1;
    }
    for (char c : s){
        if(c< '0' || c>'9'){
            return -1;
        }
        output = output * 10 + (c - '0');
    }
    return output;                             
}

SelectionPolicy* Auxiliary:: translatePolicy(const string &newPolicy){
    if(newPolicy=="nve"){
        return new NaiveSelection(); 
    }
    else if(newPolicy=="bal"){
        return new BalancedSelection(0,0,0);
    }
    else if(newPolicy=="eco"){
        return new EconomySelection();
        
    }
    else if(newPolicy=="env"){
        return new SustainabilitySelection();
    }
    else{
        return nullptr;
    }
}

SettlementType Auxiliary::intToSettType(int i){
    if(i==2){
        return SettlementType::METROPOLIS;
    }
    else if(i==1){
        return SettlementType::CITY;
    }
    else{
        return SettlementType::VILLAGE;
    }
}

const FacilityCategory Auxiliary:: intToFacCategory(int i){
    if(i==2){
        return FacilityCategory::ENVIRONMENT;
    }
    else if(i==1){
        return FacilityCategory::ECONOMY;
    }
    else{
        return FacilityCategory::LIFE_QUALITY;
    }
}

string Auxiliary::intToStrPolic(int i){
    if(i==0){
        return "nve";
    }
    else if(i==1){
        return "bal";
    }
    else if(i==2){
        return "eco";
    }
    else if(i==3){
        return "env";
    }
    else{
        return "none";
    }
}
