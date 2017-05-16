#ifndef DVCS_CONVOL_COEFF_FUNCTION_MODULE_H
#define DVCS_CONVOL_COEFF_FUNCTION_MODULE_H

/**
 * @file DVCSConvolCoeffFunctionModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date July 22, 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <complex>
#include <map>
#include <string>

#include "../../../beans/automation/BaseObjectData.h"
#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/List.h"
#include "../../../beans/PerturbativeQCDOrderType.h"
#include "../../ConvolCoeffFunctionModule.h"

class ActiveFlavorsModule;
class DVCSConvolCoeffFunctionResult;
class RunningAlphaStrongModule;

/**
 * @class DVCSConvolCoeffFunctionModule
 *
 * @brief
 */
class DVCSConvolCoeffFunctionModule: public ConvolCoeffFunctionModule {
public:
    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     */
    DVCSConvolCoeffFunctionModule(const std::string &className);
    /**
     * Default destructor.
     */
    virtual ~DVCSConvolCoeffFunctionModule();

    virtual DVCSConvolCoeffFunctionModule* clone() const = 0;

    virtual void resolveObjectDependencies();

    virtual void configure(const ElemUtils::Parameters &parameters);

    virtual std::complex<double> compute(double xi, double t, double Q2,
            double MuF2, double MuR2, GPDType::Type gpdType);

    /**
     * Method to compute some CFFs.
     * Can be implemented and used in child class, if needed.
     * @return Complex result.
     */
    virtual std::complex<double> computeUnpolarized();
    /**
     * Method to compute some CFFs.
     * Can be implemented and used in child class, if needed.
     * @return Complex result.
     */
    virtual std::complex<double> computePolarized();
    /**
     * Method to compute some CFFs.
     * Can be implemented and used in child class, if needed.
     * @return Complex result.
     */
    virtual std::complex<double> computeCFF();

    virtual List<GPDType> getListOfAvailableGPDTypeForComputation() const;

    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

    // ##### GETTERS & SETTERS #####

    PerturbativeQCDOrderType::Type getQCDOrderType() const;
    /**
     * Defines the order of the perturbative QCD computation.
     * @param qcdOrderType
     */
    void setQCDOrderType(PerturbativeQCDOrderType::Type qcdOrderType);

    void setNfConvolCoeffFunction(ActiveFlavorsModule* pNfConvolCoeffFunction);
    void setRunningAlphaStrongModule(
            RunningAlphaStrongModule* pRunningAlphaStrongModule);

protected:

    /**
     * Copy constructor.
     *
     * @param other
     */
    DVCSConvolCoeffFunctionModule(const DVCSConvolCoeffFunctionModule &other);

    std::map<GPDType::Type,
            std::complex<double> (DVCSConvolCoeffFunctionModule::*)()> m_listOfCFFComputeFunctionAvailable;
    std::map<GPDType::Type,
            std::complex<double> (DVCSConvolCoeffFunctionModule::*)()>::iterator m_it;

    double m_xi;
    double m_t;
    double m_Q2;
    double m_MuF2;
    double m_MuR2;

    unsigned int m_nf;

    RunningAlphaStrongModule* m_pRunningAlphaStrongModule;

    ActiveFlavorsModule* m_pNfConvolCoeffFunction;

    PerturbativeQCDOrderType::Type m_qcdOrderType;
    GPDType::Type m_currentGPDComputeType;

    virtual void initModule();
    virtual void isModuleWellConfigured();

    void preCompute(double xi, double t, double Q2, double MuF2, double MuR2,
            GPDType::Type gpdType);
};

#endif /* DVCS_CONVOL_COEFF_FUNCTION_MODULE_H */
