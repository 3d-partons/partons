#ifndef DVMP_SCALES_GK_H
#define DVMP_SCALES_GK_H

/**
 * @file DVMPScalesGK.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 28 October 2015
 * @version 1.0
 */

#include <string>

#include "DVMPScalesModule.h"

namespace PARTONS {

/**
 * @class DVMPScalesGK
 *
 * @brief Evaluation of factorization in GK framework.
 *
 * This model identifies the factorization scale as \f$Q^2\f$. The renormalization scale is set to 0, as it is dynamically evaluated inside GK CFF module.
 *
 * For an example of usage of this module see the abstract class documentation.
 */
class DVMPScalesGK: public DVMPScalesModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVMPScalesGK(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVMPScalesGK(const DVMPScalesGK &other);

    /**
     * Destructor.
     */
    virtual ~DVMPScalesGK();

    virtual DVMPScalesGK* clone() const;
    virtual Scales compute(const DVMPObservableKinematic& kinematic);
};

} /* namespace PARTONS */

#endif /* DVMP_SCALES_GK_H */
