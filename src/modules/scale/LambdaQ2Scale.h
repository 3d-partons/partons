#ifndef LAMBDA_Q2_SCALE_H
#define LAMBDA_Q2_SCALE_H

/**
 * @file LambdaQ2Scale.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 28 October 2015
 * @version 1.0
 *
 * @class LambdaQ2Scale
 *
 * @brief
 */

#include <string>

#include "ScaleModule.h"

class LambdaQ2Scale: public ScaleModule {
public:
    static const std::string PARAMETER_NAME_LAMBDA;

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    LambdaQ2Scale(const std::string &className);
    virtual ~LambdaQ2Scale();

    virtual LambdaQ2Scale* clone() const;

    /**
     * Provides a generic method to configure all types of modules by passing a Parameters object.
     * (See ModuleObject class for more info).
     *
     * @param parameters
     */
    virtual void configure(ParameterList parameters);

    virtual Scale compute(double Q2);

protected:
    /**
     * Copy constructor
     */
    LambdaQ2Scale(const LambdaQ2Scale &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:
    double m_lambda;
};

#endif /* LAMBDA_Q2_SCALE_H */
