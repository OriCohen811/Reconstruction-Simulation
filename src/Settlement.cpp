#include "Settlement.h"
#include <iostream>

using namespace std;

Settlement ::  Settlement(const string &name, SettlementType type): name(name) ,type(type) {}

Settlement :: Settlement(const Settlement& other): name(other.name) ,type(other.type){}

Settlement :: ~Settlement()=default;

Settlement:: Settlement(const Settlement&& other):name(other.name), type(other.type){}

Settlement* Settlement:: clone() const{return (new Settlement(*this));}

SettlementType Settlement::getType() const{return type;}
const string& Settlement:: getName() const{return name;}

const string Settlement:: toString() const{return "SettlementName " + getName();}

