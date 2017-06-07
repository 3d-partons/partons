#ifndef Q2_MULTIPLIER_H
#define Q2_MULTIPLIER_H

/**
 * @file Q2Multiplier.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 28 October 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "ScaleModule.h"

namespace ElemUtils {
class Parameters;
} /* namespace ElemUtils */

/**
 * @class Q2Multiplier
 *
 * @brief Evaluation of factorization and renormalization scales as a linear function of \f$Q^2\f$.
 *
 * This model evaluates factorization and renormalization scales as a linear function of \f$Q^2\f$, i.e. <br>
 * \f$\mu_{F}^{2} = \mu_{R}^{2} = \lambda Q^2\f$ <br>
 * The value of \f$\lambda\f$ can be changed by Q2Multiplier::configure() function using Q2Multiplier::PARAMETER_NAME_LAMBDA parameter key.
 *
 * For an example of usage of this module see the abstract class documentation.
 */
class Q2Multiplier: public ScaleModule {

public:

    /**
     * Name to set value of Q2Multiplier::m_lambda via the automatization.
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
    Q2Multiplier(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~Q2Multiplier();

    virtual Q2Multiplier* clone() const;

    virtual void configure(const ElemUtils::Parameters &parameters);

    virtual Scale compute(double Q2);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    Q2Multiplier(const Q2Multiplier &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:

    /**
     * Scaling parameter.
     */
    double m_lambda;
};

#endif /* Q2_MULTIPLIER_H */
