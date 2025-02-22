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
    virtual SelectionPolicy& operator=(const SelectionPolicy& other);
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
        virtual SelectionPolicy& operator=(const SelectionPolicy& other) override;
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
        virtual ~BalancedSelection() = default;
        //
        BalancedSelection(BalancedSelection& other);
        int maxGap(const FacilityType& facility);
        virtual SelectionPolicy& operator=(const SelectionPolicy& other) override;
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
        SelectionPolicy& operator=(const SelectionPolicy& other) override;
        PolicyType getPlcType() const override;
    private:
        int lastSelectedIndex;
};

class SustainabilitySelection : public SelectionPolicy {
    public:
        //given
        SustainabilitySelection();
        SustainabilitySelection *clone() const override;
        virtual ~SustainabilitySelection() = default;
        const FacilityType& selectFacility(const vector<FacilityType>& facilitiesOptions) override;
        const string toString() const override;
        //
        SustainabilitySelection(SustainabilitySelection& other);
        SustainabilitySelection(int lastSelectedIndex);
        virtual SelectionPolicy& operator=(const SelectionPolicy& other) override;
        PolicyType getPlcType() const override;
    private:
        int lastSelectedIndex;
};