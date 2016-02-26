#ifndef MATH_INTEGRATOR_MODULE_H
#define MATH_INTEGRATOR_MODULE_H

/**
 * @file MathIntegratorModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 29 July 2014
 * @version 1.0
 *
 * @class MathIntegratorModule
 *
 * @brief
 */

namespace NumA {
class Integrator1D;
} /* namespace NumA */

class MathIntegratorModule {
public:
    MathIntegratorModule();
    virtual ~MathIntegratorModule();

protected:
    /**
     * Copy constructor
     *
     * @param other
     */
    MathIntegratorModule(const MathIntegratorModule &other);

    NumA::Integrator1D* m_mathIntegrator;
};

#endif /* MATH_INTEGRATOR_MODULE_H */
