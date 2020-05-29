/**
 * @file RunningAlphaStrongGK.h
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
 * \class RunningAlphaStrongGK
 *
 * \brief Evaluation of the strong running coupling constant as in the GK HEMP framework.
 */
class RunningAlphaStrongGK: public RunningAlphaStrongModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    RunningAlphaStrongGK(const std::string &className);

    virtual RunningAlphaStrongGK* clone() const;

    /**
     * Destructor.
     */
    virtual ~RunningAlphaStrongGK();

    virtual double compute();

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    RunningAlphaStrongGK(const RunningAlphaStrongGK &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:

    double m_cLambdaQCD; ///< Lambda QCD
};

} /* namespace PARTONS */

#endif /* RUNNING_ALPHA_STRONG_VINNIKOV_H */

