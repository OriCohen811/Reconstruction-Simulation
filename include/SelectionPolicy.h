#pragma once
#include <vector>
#include "Facility.h"
using std::vector;

enum class PolicyType
{
    NVE,
    BAL,
    ECO,
    ENV
};

class SelectionPolicy{
public:
    //given
    virtual const FacilityType &selectFacility(const vector<FacilityType> &facilitiesOptions) = 0;
    virtual const string toString() const = 0;
    virtual SelectionPolicy *clone() const = 0;
    virtual ~SelectionPolicy() = default;
    //
    SelectionPolicy();
    virtual PolicyType getPlcType() const = 0;
protected:
};

class NaiveSelection: public SelectionPolicy {
    public:
        //given
        NaiveSelection();
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        NaiveSelection* clone() const override;
        virtual ~NaiveSelection() = default;
        //
        NaiveSelection(int lastSelectedIndex);
        NaiveSelection(NaiveSelection& other);
        NaiveSelection& operator=(const NaiveSelection& other);
        PolicyType getPlcType() const override;
    private:
        int lastSelectedIndex;
};

class BalancedSelection: public SelectionPolicy {
    public:
        BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore);
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        BalancedSelection *clone() const override;
        ~BalancedSelection() = default;
        //
        BalancedSelection(BalancedSelection& other);
        int maxGap(const FacilityType& facility);
        BalancedSelection& operator=(const BalancedSelection& other);
        PolicyType getPlcType() const override;
    private:
        int LifeQualityScore;
        int EconomyScore;
        int EnvironmentScore;
};

class EconomySelection: public SelectionPolicy {
    public:
        //given
        EconomySelection();
        EconomySelection *clone() const override;
        virtual ~EconomySelection() = default;
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        //
        EconomySelection(int lastSelectedIndex);
        EconomySelection(EconomySelection& other);
        EconomySelection& operator=(const EconomySelection& other);
        PolicyType getPlcType() const override;
    private:
        int lastSelectedIndex;
};

class SustainabilitySelection : public SelectionPolicy {
    public:
        //given
        SustainabilitySelection();
        SustainabilitySelection *clone() const override;
        ~SustainabilitySelection() = default;
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        //
        SustainabilitySelection(SustainabilitySelection& other);
        SustainabilitySelection(int lastSelectedIndex);
        SustainabilitySelection& operator=(const SustainabilitySelection& other);
        PolicyType getPlcType() const override;
    private:
        int lastSelectedIndex;
};