#ifndef TCS_SCALES_Q2PRIM_MULTIPLIER_H
#define TCS_SCALES_Q2PRIM_MULTIPLIER_H

/**
 * @file TCSScalesQ2PrimMultiplier.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 28 October 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "TCSScalesModule.h"

namespace PARTONS {

/**
 * @class TCSScalesQ2PrimMultiplier
 *
 * @brief Evaluation of factorization and renormalization scales as a linear function of \f$Q'^2\f$.
 *
 * This model evaluates factorization and renormalization scales as a linear function of \f$Q'^2\f$, i.e. <br>
 * \f$\mu_{F}^{2} = \mu_{R}^{2} = \lambda Q'^2\f$ <br>
 * The value of \f$\lambda\f$ can be changed by TCSScalesQ2PrimMultiplier::configure() function using TCSScalesQ2PrimMultiplier::PARAMETER_NAME_LAMBDA parameter key.
 *
 * For an example of usage of this module see the abstract class documentation.
 */
class TCSScalesQ2PrimMultiplier: public TCSScalesModule {

public:

    /**
     * Name to set value of TCSScalesQ2PrimMultiplier::m_lambda via the automatization.
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
    TCSScalesQ2PrimMultiplier(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    TCSScalesQ2PrimMultiplier(const TCSScalesQ2PrimMultiplier &other);

    /**
     * Destructor.
     */
    virtual ~TCSScalesQ2PrimMultiplier();

    virtual TCSScalesQ2PrimMultiplier* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual Scales compute(const TCSObservableKinematic& kinematic);

private:

    /**
     * Scaling parameter.
     */
    double m_lambda;
};

} /* namespace PARTONS */

#endif /* TCS_SCALES_Q2PRIM_MULTIPLIER_H */
