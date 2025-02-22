#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

class Facility;

enum class SettlementType {
    VILLAGE,
    CITY,
    METROPOLIS,
};

class Settlement {
    public:
        //given
        Settlement(const string& name, SettlementType type);
        const string &getName() const;
        SettlementType getType() const;
        const string toString() const;
        //
        Settlement(const Settlement& other);
        Settlement(const Settlement&& other);
        Settlement& operator=(Settlement& other)=delete; //operator '=' isn't relevant because name is const
        Settlement& operator=(Settlement&& other)=delete;
        ~Settlement();
        Settlement* clone() const;

        private:
            const string name;
            SettlementType type;
};