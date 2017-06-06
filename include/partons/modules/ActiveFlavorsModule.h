#ifndef ACTIVE_FLAVORS_MODULE_H
#define ACTIVE_FLAVORS_MODULE_H

/**
 * @file ActiveFlavorsModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date August 07, 2015
 * @version 1.0
 */

#include <map>
#include <string>
#include <vector>

#include "../beans/active_flavors/NfInterval.h"
#include "../beans/automation/BaseObjectData.h"
#include "../ModuleObject.h"

class NfInterval;

/**
 * @class ActiveFlavorsModule
 *
 * @brief Abstract class for modules defining number of quark flavors intervals.
 *
 * This class acts as an abstract (mother) class for modules that define the number of quark flavors intervals.
 * These intervals are used e.g. by the evolution modules to evaluate the number of active quark flavors for a given factorization scale.
 */
class ActiveFlavorsModule: public ModuleObject {

public:

    /**
     * Type of module name used by the automatization.
     */
    static const std::string ACTIVE_FLAVORS_MODULE_CLASS_NAME;

    /**
     * Constructor.
     * @param className Name of class.
     */
    ActiveFlavorsModule(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~ActiveFlavorsModule();

    virtual ActiveFlavorsModule* clone() const = 0;
    virtual void resolveObjectDependencies();

    /**
     * Add a new interval.
     * @param nfValue Number of active quark flavors in the new interval.
     * @param lowerBound Lower bound of the new interval.
     * @param upperBound Upper bound of the new interval.
     */
    void addNfInterval(unsigned short nfValue, double lowerBound,
            double upperBound);

    /**
     * Get intervals for a specific range of the factorization scale.
     * @param MuMin Factorization scale minimum value.
     * @param MuMax Factorization scale maximum value.
     * @return If lowerBound > upperBound a single interval with the number of active flavors corresponding to lowerBound is returned. Otherwise, a vector containing intervals for the requested range of factorization scale.
     */
    std::vector<NfInterval> getNfIntervals(double MuMin, double MuMax);

    /**
     * Get interval for a specific value of the factorization scale.
     * @param Mu Factorization scale value.
     * @return Requested interval. If Mu is smaller than the lower bound of the first defined interval returns this interval. If Mu is larger than the upper bound of the last defined interval returns this interval.
     */
    NfInterval getNfInterval(double Mu);

    virtual std::string toString() const;
    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    ActiveFlavorsModule(const ActiveFlavorsModule &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:

    /**
     * Vector containing defined integrals.
     */
    std::vector<NfInterval> m_nfFunctionOfMu;

    /**
     * Perform a set of test in order to check the consistency of defined integrals.
     */
    void checkCurveIntegrity();
};

#endif /* ACTIVE_FLAVORS_MODULE_H */
