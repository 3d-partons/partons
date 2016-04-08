#ifndef ACTIVE_FLAVORS_MODULE_H
#define ACTIVE_FLAVORS_MODULE_H

/**
 * @file ActiveFlavorsModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 07 August 2015
 * @version 1.0
 *
 * @class ActiveFlavorsModule
 *
 * @brief
 */

#include <string>
#include <vector>

#include "../beans/active_flavors/NfInterval.h"
#include "../ModuleObject.h"

class NfInterval;

class ActiveFlavorsModule: public ModuleObject {
public:
    ActiveFlavorsModule(const std::string &className);
    virtual ~ActiveFlavorsModule();
    /**
     * Clone
     *
     * @return
     */
    virtual ActiveFlavorsModule* clone() const = 0;

    virtual void resolveObjectDependencies();

    void addNfInterval(unsigned short nfValue, double lowerBound,
            double upperBound);
    std::vector<NfInterval> getNfIntervals(double MuMin, double MuMax);
    NfInterval getNfInterval(double Mu);

    /**
     * Return a pre-formatted characters string for output visualization of class member's values
     *
     * @return a pre-formatted characters string
     */
    virtual std::string toString();

protected:
    /**
     * Copy constructor
     */
    ActiveFlavorsModule(const ActiveFlavorsModule &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:
    std::vector<NfInterval> m_nfFunctionOfMu;

    void checkCurveIntegrity();
};

#endif /* ACTIVE_FLAVORS_MODULE_H */
