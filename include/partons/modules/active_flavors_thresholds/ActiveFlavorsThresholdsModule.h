#ifndef ACTIVE_FLAVORS_THRESHOLDS_MODULE_H
#define ACTIVE_FLAVORS_THRESHOLDS_MODULE_H

/**
 * @file ActiveFlavorsThresholdsModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date August 07, 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <map>
#include <string>
#include <vector>

#include "../../beans/active_flavors_thresholds/ActiveFlavorsThresholds.h"
#include "../../beans/automation/BaseObjectData.h"
#include "../../ModuleObject.h"

namespace PARTONS {

/**
 * @class ActiveFlavorsThresholdsModule
 *
 * @brief Abstract class for modules defining number of quark flavors intervals.
 *
 * This class acts as an abstract (mother) class for modules that define the number of quark flavors intervals.
 * These intervals are used e.g. by the evolution modules to evaluate the number of active quark flavors for a given factorization scale squared.
 */
class ActiveFlavorsThresholdsModule: public ModuleObject {

public:

    /**
     * Type of module name used by the automatization.
     */
    static const std::string ACTIVE_FLAVORS_THRESHOLDS_MODULE_CLASS_NAME;

    /**
     * Constructor.
     * @param className Name of class.
     */
    ActiveFlavorsThresholdsModule(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~ActiveFlavorsThresholdsModule();

    virtual ActiveFlavorsThresholdsModule* clone() const = 0;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void resolveObjectDependencies();
    virtual std::string toString() const;
    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

    /**
     * Get interval for a specific value of the factorization scale squared.
     * @param Mu2 Factorization scale squared value.
     * @return Requested interval. If Mu2 is smaller than the lower bound of the first defined interval returns this interval. If Mu2 is larger than the upper bound of the last defined interval returns this interval.
     */
    ActiveFlavorsThresholds getNfInterval(double Mu2) const;

    /**
     * Get intervals for a specific range of the factorization scale squared.
     * @param Mu2Min Factorization scale squared minimum value.
     * @param Mu2Max Factorization scale squared maximum value.
     * @return If lowerBound > upperBound a single interval with the number of active flavors corresponding to lowerBound is returned. Otherwise, a vector containing intervals for the requested range of factorization scale.
     */
    std::vector<ActiveFlavorsThresholds> getNfIntervals(double Mu2Min,
            double Mu2Max) const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    ActiveFlavorsThresholdsModule(const ActiveFlavorsThresholdsModule &other);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    /**
     * Add a new interval.
     * @param nfValue Number of active quark flavors in the new interval.
     * @param lowerBound Lower bound of the new interval.
     * @param upperBound Upper bound of the new interval.
     */
    void addNfInterval(unsigned short nfValue, double lowerBound,
            double upperBound);

    /**
     * Reset vector containing defined integrals.
     */
    void reset();

private:

    /**
     * Vector containing defined integrals.
     */
    std::vector<ActiveFlavorsThresholds> m_nfFunctionOfMu2;

    /**
     * Perform a set of test in order to check the consistency of defined integrals.
     */
    void checkCurveIntegrity();
};

} /* namespace PARTONS */

#endif /* ACTIVE_FLAVORS_THRESHOLDS_MODULE_H */
