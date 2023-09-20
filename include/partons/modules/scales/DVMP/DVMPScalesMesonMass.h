#ifndef DVMP_SCALES_MESON_MASS_H
#define DVMP_SCALES_MESON_MASS_H

/**
 * @file DVMPScalesMesonMass.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 28 October 2015
 * @version 1.0
 */

#include <string>

#include "DVMPScalesModule.h"

namespace PARTONS {

/**
 * @class DVMPScalesMesonMass
 *
 * @brief Evaluation of factorization and renormalization scales as meson mass.
 *
 * This model evaluates factorization and renormalization scales as meson mass.
 */
class DVMPScalesMesonMass: public DVMPScalesModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVMPScalesMesonMass(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVMPScalesMesonMass(const DVMPScalesMesonMass &other);

    /**
     * Destructor.
     */
    virtual ~DVMPScalesMesonMass();

    virtual DVMPScalesMesonMass* clone() const;
    virtual Scales compute(const DVMPObservableKinematic& kinematic);

};

} /* namespace PARTONS */

#endif /* DVMP_SCALES_MESON_MASS_H */
