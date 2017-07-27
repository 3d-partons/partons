#ifndef SCALES_H
#define SCALES_H

/**
 * @file Scales.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 07 October 2015
 * @version 1.0
 */

#include <string>

#include "../BaseObject.h"

namespace PARTONS {

/**
 * @class Scales
 *
 * @brief Container to store square values of factorization and renormalization scales.
 *
 * This class acts as a container to store square values of factorization and renormalization scales. It is used e.g. by ScaleModule class and its derivatives. E.g.
 \code{.cpp}
 //get one of scales modules
 //scale modules are used to evaluate factorization and renormalization scales from given Q2 value
 ScaleModule* pQ2Multiplier = Partons::getInstance()->getModuleObjectFactory()->newScaleModule(Q2Multiplier::classId);

 //Q2Multiplier model does it by multiplying Q2 by given lambda parameter. It means that the factorization and renormalization scales always the same
 //E.g.
 double Q2 = 2.;

 Scales scales1 = pQ2Multiplier->compute(Q2);

 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Factorization scale squared is: " << scales1.getMuF2() << " GeV2");
 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Renormalization scale squared is: " << scales1.getMuR2() << " GeV2");

 //let us change lambda parameter and recalculate scales
 pQ2Multiplier->configure(ElemUtils::Parameters(Q2Multiplier::PARAMETER_NAME_LAMBDA, 2.));

 Scales scales2 = pQ2Multiplier->compute(Q2);

 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Factorization scale squared is: " << scales2.getMuF2() << " GeV2");
 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Renormalization scale squared is: " << scales2.getMuR2() << " GeV2");
 \endcode
 which gives via Logger:
 \code
 20-05-2017 11:15:14 [INFO] (example::main) Factorization scale squared is: 2 GeV2
 20-05-2017 11:15:14 [INFO] (example::main) Renormalization scale squared is: 2 GeV2
 20-05-2017 11:15:14 [INFO] (Q2Multiplier::configure) lambda configured with value = 2
 20-05-2017 11:15:14 [INFO] (example::main) Factorization scale squared is: 4 GeV2
 20-05-2017 11:15:14 [INFO] (example::main) Renormalization scale squared is: 4 GeV2
 \endcode
 */
class Scales: public BaseObject {
public:

    /**
     * Default constructor.
     */
    Scales();

    /**
     * Assignment constructor.
     * @param MuF2 Squared value of factorization scale to be set.
     * @param MuR2 Squared value of renormalization scale to be set.
     */
    Scales(double MuF2, double MuR2);

    /**
     * Destructor.
     */
    virtual ~Scales();

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get squared value of factorization scale.
     */
    double getMuF2() const;

    /**
     * Set squared value of factorization scale.
     */
    void setMuF2(double muF2);

    /**
     * Get squared value of renormalization scale.
     */
    double getMuR2() const;

    /**
     * Set squared value of renormalization scale.
     */
    void setMuR2(double muR2);

    virtual std::string toString() const;

private:

    /**
     * Squared value of factorization scale.
     */
    double m_MuF2;

    /**
     * Squared value of renormalization scale.
     */
    double m_MuR2;
};

} /* namespace PARTONS */

#endif /* SCALES_H */
