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

#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <vector>

namespace ElemUtils {
class Parameters;
} /* namespace ElemUtils */
namespace NumA {
class FunctionType1D;
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

    void setIntegrator(NumA::IntegratorType1D::Type integratorType);

    double integrate(NumA::FunctionType1D* pFunction, double a, double b,
            std::vector<double> &parameters);

    void configureIntegrator(const ElemUtils::Parameters &parameters);

    NumA::Integrator1D* getMathIntegrator();

private:
    NumA::Integrator1D* m_mathIntegrator;
};

#endif /* MATH_INTEGRATOR_MODULE_H */
