#ifndef Q2_MULTIPLIER_H
#define Q2_MULTIPLIER_H

/**
 * @file Q2Multiplier.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 28 October 2015
 * @version 1.0
 *
 * @class Q2Multiplier
 *
 * @brief
 */

#include <string>

#include "ScaleModule.h"

class Q2Multiplier: public ScaleModule {
public:
    static const std::string PARAMETER_NAME_LAMBDA;

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    Q2Multiplier(const std::string &className);
    virtual ~Q2Multiplier();

    virtual Q2Multiplier* clone() const;

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
    Q2Multiplier(const Q2Multiplier &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:
    double m_lambda;
};

#endif /* Q2_MULTIPLIER_H */
