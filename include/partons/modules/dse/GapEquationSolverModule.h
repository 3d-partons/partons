/**
 * @file GapEquationSolverModule.h
 * @author Nabil CHOUIKA (SPhN / CEA Saclay)
 * @date Jan 22, 2016
 * @version 1.0
 *
 * @class GapEquationSolverModule
 */

#ifndef GAPEQUATIONSOLVER_H_
#define GAPEQUATIONSOLVER_H_

#include <string>

#include "../../ModuleObject.h"

class GapEquationSolverModule: public ModuleObject {
public:
    /**
     * Default constructor
     */
    GapEquationSolverModule(const std::string &className);

    virtual GapEquationSolverModule* clone() const = 0;

    /**
     * Default destructor
     */
    virtual ~GapEquationSolverModule();

    /**
     * Provides a generic method to configure all types of modules by passing a Parameters object.
     * (See ModuleObject class for more info).
     *
     * @param parameters
     */
    virtual void configure(ParameterList parameters);

protected:
    /**
     * Copy constructor
     *
     * @param other
     */
    GapEquationSolverModule(const GapEquationSolverModule &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();
};

#endif /* GAPEQUATIONSOLVER_H_ */
