#ifndef DVCS_SCALES_Q2_MULTIPLIER_H
#define DVCS_SCALES_Q2_MULTIPLIER_H

/**
 * @file DVCSScalesQ2Multiplier.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 28 October 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "DVCSScalesModule.h"

namespace PARTONS {

/**
 * @class DVCSScalesQ2Multiplier
 *
 * @brief Evaluation of factorization and renormalization scales as a linear function of \f$Q^2\f$.
 *
 * This model evaluates factorization and renormalization scales as a linear function of \f$Q^2\f$, i.e. <br>
 * \f$\mu_{F}^{2} = \mu_{R}^{2} = \lambda Q^2\f$ <br>
 * The value of \f$\lambda\f$ can be changed by DVCSScalesQ2Multiplier::configure() function using DVCSScalesQ2Multiplier::PARAMETER_NAME_LAMBDA parameter key.
 *
 * For an example of usage of this module see the abstract class documentation.
 */
class DVCSScalesQ2Multiplier: public DVCSScalesModule {

public:

    /**
     * Name to set value of DVCSScalesQ2Multiplier::m_lambda via the automatization.
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
    DVCSScalesQ2Multiplier(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSScalesQ2Multiplier(const DVCSScalesQ2Multiplier &other);

    /**
     * Destructor.
     */
    virtual ~DVCSScalesQ2Multiplier();

    virtual DVCSScalesQ2Multiplier* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual Scales compute(const DVCSObservableKinematic& kinematic);

protected:

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:

    /**
     * Scaling parameter.
     */
    double m_lambda;
};

} /* namespace PARTONS */

#endif /* DVCS_SCALES_Q2_MULTIPLIER_H */
