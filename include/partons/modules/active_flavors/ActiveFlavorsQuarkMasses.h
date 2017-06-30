#ifndef ACTIVEFLAVORSQUARKMASSES_H
#define ACTIVEFLAVORSQUARKMASSES_H

/**
 * @file ActiveFlavorsQuarkMasses.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 07 August 2015
 * @version 1.0
 */

#include <string>

#include "../ActiveFlavorsModule.h"

/**
 * @class ActiveFlavorsQuarkMasses
 *
 * @brief Number of active quark flavors intervals corresponding to quark masses.
 *
 * This module defines number of active quark flavors intervals corresponding to quark masses. The intervals are:
 N. of active flavors  | Min \f$\mu_{F}^{2}\f$ [GeV\f$^2\f$] | Max \f$\mu_{F}^{2}\f$ [GeV\f$^2\f$]
 ------------- | ------------- | -------------
 1  | 0. | QUARK_UP_MASS\f$^2\f$
 2  | QUARK_UP_MASS\f$^2\f$ | QUARK_DOWN_MASS\f$^2\f$
 3  | QUARK_DOWN_MASS\f$^2\f$ | QUARK_STRANGE_MASS\f$^2\f$
 4  | QUARK_STRANGE_MASS\f$^2\f$ | QUARK_CHARM_MASS\f$^2\f$
 5  | QUARK_CHARM_MASS\f$^2\f$ | QUARK_BOTTOM_MASS\f$^2\f$
 6  | QUARK_BOTTOM_MASS\f$^2\f$ | QUARK_TOP_MASS\f$^2\f$
 * The quark masses are defined in `FundamentalPhysicalConstants.h` file.
 */
class ActiveFlavorsQuarkMasses: public ActiveFlavorsModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of this class.
     */
    ActiveFlavorsQuarkMasses(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~ActiveFlavorsQuarkMasses();

    virtual ActiveFlavorsQuarkMasses* clone() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    ActiveFlavorsQuarkMasses(const ActiveFlavorsQuarkMasses &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();
};

#endif /* ACTIVEFLAVORSQUARKMASSES_H */
