#ifndef BASE_MODULE_H
#define BASE_MODULE_H

/**
 * @file BaseModule.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 06 Aout 2014
 * @version 1.0
 *
 * Last update : 20 Aout 2014
 *
 * @class BaseModule
 * @brief Super class of all module types - A module is a class that performs a specific compute task
 *
 * ex : GK11Model is a module that computes only GPDq(x, xi, t, MuF, MuR) defined by Kroll-Goloskokov in 2011 \n
 * ex : DVCSCFFModule is a module that evaluates the convolution of the GPD H with the hard scattering kernel at twist 2 necessary to the evaluation of DVCS scattering amplitudes.
 */

#include <stddef.h>
#include <string>
#include <vector>

#include "BaseObject.h"

class BaseModule: public BaseObject {
public:
    static const std::string className;

    /**
     * Default constructor
     */
    BaseModule(std::string _ID);

    /**
     * Default destructor
     */
    virtual ~BaseModule();

    /**
     * Return the number of current parameters used for configure this module.
     */
    size_t getNbOfParameters();

// ################   GETTERS & SETTERS   ################

    const std::vector<double>& getParameters() const;
    void setParameters(const std::vector<double>& parameters);
    const std::string& getID() const;

protected:
    std::vector<double> parameters; ///< parameters used for configure this module.

private:

    std::string ID;
};

#endif /* BASE_MODULE_H */
