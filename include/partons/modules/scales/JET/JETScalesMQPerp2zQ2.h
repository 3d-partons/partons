#ifndef JET_SCALES_Q2_MULTIPLIER_H
#define JET_SCALES_Q2_MULTIPLIER_H

/**
 * @file JETScalesMQPerp2zQ2.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 28 October 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "JETScalesModule.h"

namespace PARTONS {

/**
 * @class JETScalesMQPerp2zQ2
 *
 * @brief Evaluation of factorization and renormalization scales from \f$m\f$,  \f$q_{\perp}^2\f$,  \f$z\f$ and  \f$Q^2\f$.
 *
 * This model evaluates factorization and renormalization scales as <br>
 * \f$\mu_{F}^{2} = \mu_{R}^{2} = m^2 + q_{\perp}^2 + z \bar{z} Q^2\f$ <br>
 *
 * For an example of usage of this module see the abstract class documentation.
 */
class JETScalesMQPerp2zQ2: public JETScalesModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    JETScalesMQPerp2zQ2(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    JETScalesMQPerp2zQ2(const JETScalesMQPerp2zQ2 &other);

    /**
     * Destructor.
     */
    virtual ~JETScalesMQPerp2zQ2();

    virtual JETScalesMQPerp2zQ2* clone() const;
    virtual Scales compute(const JETObservableKinematic& kinematic);
};

} /* namespace PARTONS */

#endif /* JET_SCALES_Q2_MULTIPLIER_H */
