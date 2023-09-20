#ifndef DVMP_CFF_ISSK15_H
#define DVMP_CFF_ISSK15_H

/**
 * @file DVMPCFFISSK15.h
 * @author Kemal Tezgin
 * @date October 21, 2019
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <complex>
#include <map>
#include <string>
#include <vector>

#include "../../../beans/automation/BaseObjectData.h"
#include "DVMPConvolCoeffFunctionModule.h"

namespace PARTONS {

/**
 * @class DVMPCFFISSK15
 *
 * This module calculates helicity amplitudes and partial cross sections of exclusive J/Psi
 * photoproduction formulated in https://arxiv.org/pdf/hep-ph/0401131.pdf
 */
class DVMPCFFISSK15: public DVMPConvolCoeffFunctionModule {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     * @param className Name of last child class.
     */
    DVMPCFFISSK15(const std::string &className);

    virtual DVMPCFFISSK15* clone() const;

    /**
     * Default destructor.
     */
    virtual ~DVMPCFFISSK15();

    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void resolveObjectDependencies();
    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

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

protected:

    /**
     * Copy constructor.
     * @param other
     */
    DVMPCFFISSK15(const DVMPCFFISSK15 &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    virtual std::complex<double> computeCFF();

private:

    RunningAlphaStrongModule* m_pRunningAlphaStrongModule; ///< AlphaS module.
    double m_gpdResultXiXiPlus; ///< GPD(+) result at x=xi.

    double reGluonLO(double x, std::vector<double> params); ///< Integrand LO real part.
    NumA::FunctionType1D* m_pReGluonLO;  ///< Integrator.

    void initFunctorsForIntegrations(); ///< Initialize functors.
};

} /* namespace PARTONS */

#endif /* DVMP_CFF_ISSK15_H */
