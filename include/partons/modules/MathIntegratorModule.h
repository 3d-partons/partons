#ifndef MATH_INTEGRATOR_MODULE_H
#define MATH_INTEGRATOR_MODULE_H

/**
 * @file MathIntegratorModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 29 July 2014
 * @version 1.0
 */

#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <string>
#include <vector>

namespace ElemUtils {
class Parameters;
} /* namespace ElemUtils */
namespace NumA {
class FunctionType1D;
class Integrator1D;
} /* namespace NumA */

/**
 * @class MathIntegratorModule
 *
 * @brief Wrapper for integration routines. Can be inherited by a module that uses integration.
 */
class MathIntegratorModule {
public:
    static const std::string PARAM_NAME_INTEGRATOR_TYPE; ///< Parameter used in configureIntegrator() or XML automation to set the type of integrator.

    /**
     * Default constructor.
     */
    MathIntegratorModule();
    /**
     * Default destructor.
     */
    virtual ~MathIntegratorModule();

protected:
    /**
     * Copy constructor.
     *
     * @param other
     */
    MathIntegratorModule(const MathIntegratorModule &other);

    /**
     * Sets a given integrator available in NumA++.
     * @param integratorType
     */
    void setIntegrator(NumA::IntegratorType1D::Type integratorType);

    /**
     * Computes the integral of a function defined via a functor.
     * @param pFunction Functor.
     * @param a Lower bound.
     * @param b Upper bound.
     * @param parameters List of parameters needed by the function.
     * @return Integral.
     */
    double integrate(NumA::FunctionType1D* pFunction, double a, double b,
            std::vector<double> &parameters);

    /**
     * Provides a generic method to configure all types of integration routines by passing a Parameters object.
     * Parameters class represents a list of couples key/value (see Parameters class documentation for more info).
     * Can also be used to set the integrator via the key PARAM_NAME_INTEGRATOR_TYPE.
     * @param parameters
     */
    void configureIntegrator(const ElemUtils::Parameters &parameters);

    /**
     *
     * @return Integration routine. Pointer to NumA::Integrator1D object.
     */
    NumA::Integrator1D* getMathIntegrator();

private:
    NumA::Integrator1D* m_mathIntegrator; ///< Integration routine. Pointer to NumA::Integrator1D object.
};

#endif /* MATH_INTEGRATOR_MODULE_H */
