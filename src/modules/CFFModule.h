#ifndef CFF_MODULE_H
#define CFF_MODULE_H

/**
 * @file CFFModule.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 07 Aout 2014
 * @version 1.0
 *
 * Last update : 22 September 2014
 *
 * @class CFFModule
 * @brief Abstract class that provides skeleton to implement a Campton Form Factor (CFF) module.
 */

#include <complex>
#include <string>

#include "../beans/QCDOrderType.h"
#include "ModuleObject.h"

class GPDModule;
class GPDService;

class CFFModule: public ModuleObject {
public:
    /**
     * Default constructor
     */
    CFFModule(std::string _ID);

    /**
     * Copy constructor
     *
     * @param other
     */
    CFFModule(const CFFModule &other);

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
    virtual std::complex<double> compute(const double &xB, const double &t,
            const double Q2, const double &MuF, const double &MuR,
            const QCDOrderType &qcdOrderType) = 0;

    // ##### GETTERS & SETTERS #####

    void setGPDModule(GPDModule* gpdModule);
    void setQCDOrderType(QCDOrderType* qcdOrderType);

protected:
    //TODO expliquer pourquoi CFF en a besoin
    /**
     * A pointer to GPDService.
     */
    GPDService* m_pGPDService;

    GPDModule* m_pGPDModule;
    QCDOrderType* m_qcdOrderType;

private:

};

#endif /* CFF_MODULE_H */
