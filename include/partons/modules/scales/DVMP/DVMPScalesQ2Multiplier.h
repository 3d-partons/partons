#ifndef DVMP_SCALES_Q2_MULTIPLIER_H
#define DVMP_SCALES_Q2_MULTIPLIER_H

/**
 * @file DVMPScalesQ2Multiplier.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 28 October 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "DVMPScalesModule.h"

namespace PARTONS {

/**
 * @class DVMPScalesQ2Multiplier
 *
 * @brief Evaluation of factorization and renormalization scales as a linear function of \f$Q^2\f$.
 *
 * This model evaluates factorization and renormalization scales as a linear function of \f$Q^2\f$, i.e. <br>
 * \f$\mu_{F}^{2} = \mu_{R}^{2} = \lambda Q^2\f$ <br>
 * The value of \f$\lambda\f$ can be changed by DVMPScalesQ2Multiplier::configure() function using DVMPScalesQ2Multiplier::PARAMETER_NAME_LAMBDA parameter key.
 *
 * For an example of usage of this module see the abstract class documentation.
 */
class DVMPScalesQ2Multiplier: public DVMPScalesModule {

public:

    /**
     * Name to set value of DVMPScalesQ2Multiplier::m_lambda via the automatization.
     */
    static const std::string PARAMETER_NAME_LAMBDA;

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVMPScalesQ2Multiplier(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVMPScalesQ2Multiplier(const DVMPScalesQ2Multiplier &other);

    /**
     * Destructor.
     */
    virtual ~DVMPScalesQ2Multiplier();

    virtual DVMPScalesQ2Multiplier* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual Scales compute(const DVMPObservableKinematic& kinematic);

private:

    /**
     * Scaling parameter.
     */
    double m_lambda;
};

} /* namespace PARTONS */

#endif /* DVMP_SCALES_Q2_MULTIPLIER_H */
