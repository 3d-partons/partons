#ifndef CFF_MODULE_H
#define CFF_MODULE_H

/**
 * @file CFFModule.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 07 Aout 2014
 * @version 1.0
 *
 * @class CFFModule
 * @brief Abstract class that provides skeleton to implement a Campton Form Factor (CFF) module.
 */

#include <complex>
#include <map>
#include <string>

#include "../beans/gpd/GPDComputeType.h"
#include "../beans/QCDOrderType.h"
#include "ModuleObject.h"

class CFFOutputData;
class GPDModule;

class CFFModule: public ModuleObject {
public:
    /**
     * Default constructor
     */
    CFFModule(std::string _ID);

    virtual CFFModule* clone() const = 0;

    /**
     * Default destructor
     */
    virtual ~CFFModule();

    /**
     * Virtual method. \n
     * Compute CFF with some input parameters.
     *
     * @param _xi : longitudinal momentum
     * @param _t : momentum transfer (Mandelstam variable)
     * @param Q2 : Photon virtuality (in GeV^2)
     * @param _MuF : Factorisation
     * @param _MuR : Re-normalisation
     *
     * @return Return a pair of two double values. Real and Imaginary parts of CFF.
     */
    virtual CFFOutputData compute(const double xB, const double t,
            const double Q2, const double MuF, const double MuR,
            GPDComputeType::Type gpdComputeType) = 0;

    virtual std::complex<double> computeUnpolarized() = 0;

    virtual std::complex<double> computePolarized() = 0;

    // ##### GETTERS & SETTERS #####

    const GPDModule* getGpdModule() const;
    void setGpdModule(GPDModule* gpdModule);
    QCDOrderType::Type getQcdOrderType() const;
    void setQcdOrderType(QCDOrderType::Type qcdOrderType);

protected:

    /**
     * Copy constructor
     *
     * @param other
     */
    CFFModule(const CFFModule &other);

    std::map<GPDComputeType::Type, std::complex<double> (CFFModule::*)()> m_listOfCFFComputeFunctionAvailable;
    std::map<GPDComputeType::Type, std::complex<double> (CFFModule::*)()>::iterator m_it;

    double m_xi;
    double m_xB;
    double m_t;
    double m_Q2;
    double m_MuF;
    double m_MuR;

    GPDModule* m_pGPDModule;

    QCDOrderType::Type m_qcdOrderType;
    GPDComputeType::Type m_currentGPDComputeType;
};

#endif /* CFF_MODULE_H */
