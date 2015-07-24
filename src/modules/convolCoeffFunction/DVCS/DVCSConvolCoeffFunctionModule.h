#ifndef DVCS_CONVOL_COEFF_FUNCTION_MODULE_H
#define DVCS_CONVOL_COEFF_FUNCTION_MODULE_H

/**
 * @file DVCSConvolCoeffFunctionModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 22 July 2015
 * @version 1.0
 *
 * @class DVCSConvolCoeffFunctionModule
 *
 * @brief
 */

#include <complex>
#include <map>
#include <string>

#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/PerturbativeQCDOrderType.h"
#include "../../ConvolCoeffFunctionModule.h"

class DVCSConvolCoeffFunctionResult;
class GPDModule;
class RunningAlphaStrongModule;

class DVCSConvolCoeffFunctionModule: public ConvolCoeffFunctionModule {
public:
    DVCSConvolCoeffFunctionModule(const std::string &className);
    virtual ~DVCSConvolCoeffFunctionModule();

    virtual DVCSConvolCoeffFunctionResult compute(double xi, double t,
            double Q2, double MuF2, double MuR2, GPDType::Type gpdType);

    virtual std::complex<double> computeUnpolarized();
    virtual std::complex<double> computePolarized();

    // ##### GETTERS & SETTERS #####

    const GPDModule* getGPDModule() const;
    void setGPDModule(GPDModule* gpdModule);
    PerturbativeQCDOrderType::Type getQCDOrderType() const;
    void setQCDOrderType(PerturbativeQCDOrderType::Type qcdOrderType);

protected:

    /**
     * Copy constructor
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

    GPDModule* m_pGPDModule;
    RunningAlphaStrongModule* m_pRunningAlphaStrongModule;

    PerturbativeQCDOrderType::Type m_qcdOrderType;
    GPDType::Type m_currentGPDComputeType;
    GPDType::Type m_gpdType;

    virtual void initModule();
    virtual void isModuleWellConfigured();

    void preCompute(double xi, double t, double Q2, double MuF2, double MuR2,
            GPDType::Type gpdType);
};

#endif /* DVCS_CONVOL_COEFF_FUNCTION_MODULE_H */
