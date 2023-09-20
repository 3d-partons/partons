#ifndef DVMP_PROCESS_ISSK15_H
#define DVMP_PROCESS_ISSK15_H

/**
 * @file DVMPProcessISSK15.h
 * @author Kemal Tezgin
 * @date October 21, 2019
 * @version 1.0
 */

#include <map>
#include <string>

#include "../../../beans/automation/BaseObjectData.h"
#include "../../../utils/type/PhysicalType.h"
#include "DVMPProcessModule.h"

namespace PARTONS {
class RunningAlphaStrongModule;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class DVMPProcessISSK15
 *
 * TODO
 */
class DVMPProcessISSK15: public DVMPProcessModule {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     */
    DVMPProcessISSK15(const std::string &className);

    /**
     * Default destructor.
     */
    virtual ~DVMPProcessISSK15();
    virtual DVMPProcessISSK15* clone() const;

protected:

    /** Copy constructor.
     @param other Object to be copied.
     */
    DVMPProcessISSK15(const DVMPProcessISSK15& other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    virtual void resolveObjectDependencies();
    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

    virtual PhysicalType<double> CrossSection();

    //*** SETTERS AND GETTERS ******************************************

    /**
     * Get alphaS module.
     */
    RunningAlphaStrongModule* getRunningAlphaStrongModule() const;

    /**
     * Set alphaS module.
     */
    void setRunningAlphaStrongModule(
            RunningAlphaStrongModule* pRunningAlphaStrongModule);

private:

    double m_W2; ///< W^2 variable.
    double m_xi; ///< xi variable.

    double lambdaFunction(double a, double b, double c) const; ///< Mandalstam function.

    RunningAlphaStrongModule* m_pRunningAlphaStrongModule; ///< AlphaS module.
};

} /* namespace PARTONS */

#endif /* DVMP_PROCESS_ISSK15_H */
