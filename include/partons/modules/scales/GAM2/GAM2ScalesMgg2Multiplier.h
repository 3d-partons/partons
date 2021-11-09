#ifndef GAM2_SCALES_MGG2_MULTIPLIER_H
#define GAM2_SCALES_MGG2_MULTIPLIER_H

/**
 * @file GAM2ScalesMgg2Multiplier.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 28 October 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "GAM2ScalesModule.h"

namespace PARTONS {

/**
 * @class GAM2ScalesMgg2Multiplier
 *
 * @brief Evaluation of factorization and renormalization scales as a linear function of \f$M_{\gamma\gamma}^2\f$.
 *
 * This model evaluates factorization and renormalization scales as a linear function of \f$M_{\gamma\gamma}^2\f$, i.e. <br>
 * \f$\mu_{F}^{2} = \mu_{R}^{2} = \lambda M_{\gamma\gamma}^2\f$ <br>
 * The value of \f$\lambda\f$ can be changed by GAM2ScalesMgg2Multiplier::configure() function using GAM2ScalesMgg2Multiplier::PARAMETER_NAME_LAMBDA parameter key.
 *
 * For an example of usage of this module see the abstract class documentation.
 */
class GAM2ScalesMgg2Multiplier: public GAM2ScalesModule {

public:

    /**
     * Name to set value of GAM2ScalesMgg2Multiplier::m_lambda via the automatization.
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
    GAM2ScalesMgg2Multiplier(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    GAM2ScalesMgg2Multiplier(const GAM2ScalesMgg2Multiplier &other);

    /**
     * Destructor.
     */
    virtual ~GAM2ScalesMgg2Multiplier();

    virtual GAM2ScalesMgg2Multiplier* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual Scales compute(const GAM2ObservableKinematic& kinematic);

private:

    /**
     * Scaling parameter.
     */
    double m_lambda;
};

} /* namespace PARTONS */

#endif /* GAM2_SCALES_MGG2_MULTIPLIER_H */
