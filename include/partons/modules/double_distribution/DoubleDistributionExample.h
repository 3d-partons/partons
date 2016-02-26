#ifndef DOUBLE_DISTRIBUTION_EXAMPLE_H
#define DOUBLE_DISTRIBUTION_EXAMPLE_H

/**
 * @file DoubleDistributionExample.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 30 July 2015
 * @version 1.0
 *
 * @class DoubleDistributionExample
 *
 * @brief
 */

#include <string>
#include <vector>

#include "../DoubleDistributionModule.h"
#include "../MathIntegratorModule.h"

namespace NumA {
class FunctionType1D;
} /* namespace NumA */

class DoubleDistributionExample: public DoubleDistributionModule,
        public MathIntegratorModule {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    DoubleDistributionExample(const std::string &className);
    virtual ~DoubleDistributionExample();

    virtual DoubleDistributionExample* clone() const;

    virtual PartonDistribution computeF();
    virtual PartonDistribution computeG();
    virtual PartonDistribution computeK();

protected:
    /**
     * Copy constructor
     *
     * @param other
     */
    DoubleDistributionExample(const DoubleDistributionExample &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:
    double integrateExample(double x,
            std::vector<double> &parameters);

    NumA::FunctionType1D* m_pIntegrateExample;

    void initFunctorsForIntegrations();
};

#endif /* DOUBLE_DISTRIBUTION_EXAMPLE_H */
