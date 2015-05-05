#ifndef CFF_MODULE_H
#define CFF_MODULE_H

/**
 * @file CFFModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 07 Aout 2014
 * @version 1.0
 *
 * @class CFFModule
 *
 * @brief Abstract class that provides skeleton to implement a Campton Form Factor (CFF) module.
 */

#include <complex>
#include <map>
#include <string>

#include "../beans/gpd/GPDType.h"
#include "../beans/PerturbativeQCDOrderType.h"
#include "ModuleObject.h"

class RunningAlphaStrongModule;

class CFFOutputData;
class GPDModule;

class CFFModule: public ModuleObject {
public:
    /**
     * Default constructor
     */
    CFFModule(const std::string &className);

    virtual CFFModule* clone() const = 0;

    /**
     * Default destructor
     */
    virtual ~CFFModule();

    /**
     * Virtual method.
     * Compute CFF with some input parameters.
     *
     * @param xB : longitudinal momentum
     * @param t : momentum transfer (Mandelstam variable)
     * @param Q2 : Photon virtuality (in GeV^2)
     * @param MuF : Factorisation
     * @param MuR : Re-normalisation
     * @param gpdComputeType
     *
     * @return Return a pair of two double values. Real and Imaginary parts of CFF.
     */
    virtual CFFOutputData compute(double xB, double t,
            double Q2, double MuF, double MuR,
            GPDType::Type gpdType);

    virtual std::complex<double> computeUnpolarized();
    virtual std::complex<double> computePolarized();

    // ##### GETTERS & SETTERS #####

    const GPDModule* getGpdModule() const;
    void setGpdModule(GPDModule* gpdModule);
    PerturbativeQCDOrderType::Type getQcdOrderType() const;
    void setQcdOrderType(PerturbativeQCDOrderType::Type qcdOrderType);

protected:

    /**
     * Copy constructor
     *
     * @param other
     */
    CFFModule(const CFFModule &other);

    std::map<GPDType::Type, std::complex<double> (CFFModule::*)()> m_listOfCFFComputeFunctionAvailable;
    std::map<GPDType::Type, std::complex<double> (CFFModule::*)()>::iterator m_it;

    double m_xi;
    double m_xB;
    double m_t;
    double m_Q2;
    double m_MuF;
    double m_MuR;

    PerturbativeQCDOrderType::Type m_qcdOrderType;
    GPDType::Type m_currentGPDComputeType;
    GPDType::Type m_gpdType;

    GPDModule* m_pGPDModule;

    RunningAlphaStrongModule* m_pRunningAlphaStrongModule;

    virtual void initModule();
    virtual void isModuleWellConfigured();

    void preCompute(double xB, double t, double Q2,
            double MuF, double MuR, GPDType::Type gpdType);
};

#endif /* CFF_MODULE_H */
