#ifndef DDVCS_CFF_TEST_H
#define DDVCS_CFF_TEST_H

#include <complex>
#include <string>
#include <vector>

#include "DDVCSConvolCoeffFunctionModule.h"

namespace PARTONS {

/**
 * @class DDVCSCFFTEST
 *
 * @brief TODO //NEEDS TO BE UPDATED
 *
 * TODO //NEEDS TO BE UPDATED
 */
class DDVCSCFFTEST: public DDVCSConvolCoeffFunctionModule {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     */
    DDVCSCFFTEST(const std::string &className);

    virtual DDVCSCFFTEST* clone() const;

    /**
     * Destructor.
     */
    virtual ~DDVCSCFFTEST();

    virtual void resolveObjectDependencies();

protected:

    /**
     * Copy constructor.
     */
    DDVCSCFFTEST(const DDVCSCFFTEST &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    virtual std::complex<double> computeUnpolarized();
    virtual std::complex<double> computePolarized();

private:

    /**
     * Convolution.
     */
    double convolutionUnpolarized(double x, std::vector<double> params);

    /**
     * Convolution.
     */
    double convolutionPolarized(double x, std::vector<double> params);

    NumA::FunctionType1D* m_pConvolutionUnpolarized; ///< Functor to convolutionUnpolarized().
    NumA::FunctionType1D* m_pConvolutionPolarized; ///< Functor to convolutionPolarized().

    /**
     * Compute GPD combination over quark flavors.
     */
    double computeSquareChargeAveragedGPD(
            const PartonDistribution &partonDistribution) const;

    /**
     * Initialize functors.
     */
    void initFunctorsForIntegrations();

    double m_partonDistributionEtaXiSummed; ///< GPD at (eta, xi)
};

} /* namespace PARTONS */

#endif /* DDVCS_CFF_TEST_H */
