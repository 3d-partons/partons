#ifndef CFF_MODULE_H
#define CFF_MODULE_H

/**
 * @file CFFModule.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 07 Aout 2014
 * @version 1.0
 *
 * Last update : 21 Aout 2014
 *
 * @class CFFModule
 * @brief Abstract class that provides skeleton to implement a Campton Form Factor (CFF) module.
 */

#include <string>
#include <utility>

#include "BaseModule.h"

class GPDService;

class CFFModule: public BaseModule {
    //TODO expliquer pourquoi CFF en a besoin
    /**
     * A pointer to GPDService.
     */
    GPDService* pGPDService;

public:
    /**
     * Default constructor
     */
    CFFModule(std::string _ID);

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
    virtual std::pair<double, double> compute(const double &_xi,
            const double &_t, const double Q2, const double &_MuF,
            const double &_MuR) = 0;
};

#endif /* CFF_MODULE_H */
