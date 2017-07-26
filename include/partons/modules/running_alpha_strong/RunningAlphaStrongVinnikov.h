/**
 * @file RunningAlphaStrongVinnikov.h
 * @author Pawel Sznajder (NCBJ Warsaw)
 * @date 01 February 2017
 * @version 1.0
*/

#ifndef RUNNING_ALPHA_STRONG_VINNIKOV_H
#define RUNNING_ALPHA_STRONG_VINNIKOV_H

#include <string>

#include "RunningAlphaStrongModule.h"

namespace PARTONS {

/*!
 * \class RunningAlphaStrongVinnikov
 *
 * \brief Evaluation of the strong running coupling constant as in the Vinnikov evolution routines.
 *
 * This module evaluates the strong running coupling constant as in the Vinnikov evolution routines @cite Vinnikov:2006xw.
 *
 * For an example of usage of this module see the abstract class documentation.
 *
 * Note: As in the original routines, the code offers the evaluation for 3, 4 or 5 active quark flavors.
 * However, by default the evaluation is done for 3 active quark flavors only.
 */
class RunningAlphaStrongVinnikov: public RunningAlphaStrongModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    RunningAlphaStrongVinnikov(const std::string &className);

    virtual RunningAlphaStrongVinnikov* clone() const;

    /**
     * Destructor.
     */
    virtual ~RunningAlphaStrongVinnikov();

    virtual double compute();

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    RunningAlphaStrongVinnikov(const RunningAlphaStrongVinnikov &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:

    /**
     * Logarithm of \f$\Lambda_{QCD}\f$ for five active quark flavors.
     */
    double LOG_LAMBDA_5;

    /**
     * Logarithm of \f$\Lambda_{QCD}\f$ for four active quark flavors.
     */
    double LOG_LAMBDA_4;

    /**
     * Logarithm of \f$\Lambda_{QCD}\f$ for three active quark flavors.
     */
    double LOG_LAMBDA_3;
};

} /* namespace PARTONS */

#endif /* RUNNING_ALPHA_STRONG_VINNIKOV_H */

