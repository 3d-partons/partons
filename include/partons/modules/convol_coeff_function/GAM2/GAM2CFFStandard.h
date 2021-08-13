#ifndef GAM2_CFF_STANDARD_H
#define GAM2_CFF_STANDARD_H

/**
 * @file GAM2CFFStandard.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @author Hervé MOUTARDE (SPhN / CEA Saclay)
 * @author Hervé MOUTARDE (SPhN / CEA Saclay)
 * @date September 08, 2014
 * @version 1.0
 */

#include <complex>
#include <map>
#include <string>
#include <vector>

#include "../../../beans/automation/BaseObjectData.h"
#include "GAM2ConvolCoeffFunctionModule.h"

namespace NumA {
class FunctionType1D;
} /* namespace NumA */
namespace PARTONS {
class PartonDistribution;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class GAM2CFFStandard
 *
 * TODO: Add description.
 *
 * Available CFF types: H, E, Ht, Et.
 */
class GAM2CFFStandard: public GAM2ConvolCoeffFunctionModule {
public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     * @param className Name of last child class.
     */
    GAM2CFFStandard(const std::string &className);

    virtual GAM2CFFStandard* clone() const;

    /**
     * Default destructor.
     */
    virtual ~GAM2CFFStandard();

    virtual void resolveObjectDependencies();

    void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

    // ##### GETTERS & SETTERS #####

    /**
     * Get alphaS module.
     */
    RunningAlphaStrongModule* getRunningAlphaStrongModule() const;

    /**
     * Set alphaS module.
     */
    void setRunningAlphaStrongModule(
            RunningAlphaStrongModule* pRunningAlphaStrongModule);

protected:
    /**
     * Copy constructor.
     * @param other
     */
    GAM2CFFStandard(const GAM2CFFStandard &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    virtual std::complex<double> computeUnpolarized();
    virtual std::complex<double> computePolarized();

private:

    RunningAlphaStrongModule *m_pRunningAlphaStrongModule; ///< Related alphaS module.

    void initFunctorsForIntegrations(); ///< Initialize functors.

    double exampleIntegration(double x, std::vector<double> params); ///< Exemplary integration.

    NumA::FunctionType1D* m_pExampleIntegration; ///< Functor.
};

} /* namespace PARTONS */

#endif /* GAM2_CFF_STANDARD_H */
