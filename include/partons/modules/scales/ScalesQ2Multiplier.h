#ifndef SCALES_Q2_MULTIPLIER_H
#define SCALES_Q2_MULTIPLIER_H

/**
 * @file ScalesQ2Multiplier.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 28 October 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "ScalesModule.h"

namespace ElemUtils {
class Parameters;
} /* namespace ElemUtils */

/**
 * @class ScalesQ2Multiplier
 *
 * @brief Evaluation of factorization and renormalization scales as a linear function of \f$Q^2\f$.
 *
 * This model evaluates factorization and renormalization scales as a linear function of \f$Q^2\f$, i.e. <br>
 * \f$\mu_{F}^{2} = \mu_{R}^{2} = \lambda Q^2\f$ <br>
 * The value of \f$\lambda\f$ can be changed by ScalesQ2Multiplier::configure() function using ScalesQ2Multiplier::PARAMETER_NAME_LAMBDA parameter key.
 *
 * For an example of usage of this module see the abstract class documentation.
 */
class ScalesQ2Multiplier: public ScalesModule {

public:

    /**
     * Name to set value of ScalesQ2Multiplier::m_lambda via the automatization.
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
    ScalesQ2Multiplier(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~ScalesQ2Multiplier();

    virtual ScalesQ2Multiplier* clone() const;

    virtual void configure(const ElemUtils::Parameters &parameters);

    virtual Scales compute(double Q2);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    ScalesQ2Multiplier(const ScalesQ2Multiplier &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:

    /**
     * Scaling parameter.
     */
    double m_lambda;
};

#endif /* SCALES_Q2_MULTIPLIER_H */
