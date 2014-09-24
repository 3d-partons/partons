#ifndef GPD_MODULE_H
#define GPD_MODULE_H

/**
 * @file GPDModule.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 06 Aout 2014
 * @version 1.0
 *
 * Last update : 24 September 2014
 *
 * @class GPDModule
 * @brief Abstract class that provides skeleton to implement a Generalized Parton Distributions (GPD) module.
 */

#include <string>

#include "../ModuleObject.h"
#include "GPDComputeType.h"
#include "GPDOutputData.h"

class GPDModule: public ModuleObject {

public:
    /**
     * Default constructor
     */
    GPDModule(const std::string &moduleID) :
            ModuleObject(moduleID) {
    }

    /**
     * Copy constructor
     *
     * @param other
     */
    GPDModule(const GPDModule &other) :
            ModuleObject(other) {
    }

    /**
     * Default destructor
     */
    virtual ~GPDModule() {
    }

    /**
     * Clone
     *
     * @return
     */
    virtual GPDModule* clone() const = 0;

    /**
     * Virtual method. \n
     * Compute GPD with some input parameters.
     *
     * @param _x : Bjorken variable
     * @param _xi : longitudinal momentum
     * @param _t : momentum transfer (Mandelstam variable)
     * @param _MuF : Factorisation
     * @param _MuR : Re-normalisation
     * @param _gpdComputeType : H, Ht, E, Et, ... or ALL. See GPDComputeType for more details.
     *
     * @return Return results in an GPDOutputData class. \n
     * Contains GPD results for each flavor of quarks and for each GPDs (H, Ht, E, Et, ...) if computable. \n
     */
    virtual GPDOutputData compute(const double &_x, const double &_xi,
            const double &_t, const double &_MuF, const double &_MuR,
            GPDComputeType::Type _gpdComputeType = GPDComputeType::ALL) = 0;
};

#endif /* GPD_MODULE_H */
