#ifndef BASE_MODULE_H
#define BASE_MODULE_H

/**
 * @file BaseModule.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 06 Aout 2014
 * @version 1.0
 *
 * Last update : 22 September 2014
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

#include "../BaseObject.h"

class ModuleObject: public BaseObject {
public:
    /**
     * Default constructor
     */
    ModuleObject(const std::string & moduleID);

    /**
     * Default destructor
     */
    virtual ~ModuleObject();

    /**
     * Return the number of current parameters used for configure this module.
     */
    size_t getNbOfParameters();

    virtual ModuleObject* clone() const = 0;

    virtual std::string toString();

// ################   GETTERS & SETTERS   ################

    const std::vector<double>& getParameters() const;
    //TODO politique générale sur l'implementaton de cette fonction dans les fils
    virtual void setParameters(const std::vector<double>& parameters);
    const std::string& getModuleID() const;

protected:
    /***
     * Copy constructor
     * @param other
     */
    ModuleObject(const ModuleObject &other);

    virtual void initModule() = 0;
    virtual void isModuleWellConfigured() = 0;

private:

    std::vector<double> m_parameters; ///< parameters used for configure this module.

    //TODO redondance avec className dans la classe fille
    std::string m_moduleID;
};

#endif /* BASE_MODULE_H */
