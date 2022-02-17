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
 * @brief TODO
 *
 * TODO
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

    /**
     * Get epsilon.
     */
    double getEpsilon() const;

    /**
     * Set epsilon.
     */
    void setEpsilon(double epsilon);

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

    /**
     * Convolution.
     */
    double convolutionUnpolarizedL(double x, std::vector<double> params);

    NumA::FunctionType1D* m_pConvolutionUnpolarized; ///< Functor to convolutionUnpolarized().
    NumA::FunctionType1D* m_pConvolutionPolarized; ///< Functor to convolutionPolarized().
    NumA::FunctionType1D* m_pConvolutionUnpolarizedL; ///< Functor to convolutionUnpolarizedL().

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

    double m_epsilon; ///< small parameter that helps with the practical realization of PV: PV(1/x) = integral[x/(x^2 + eps)], eps -> 0
};

} /* namespace PARTONS */

#endif /* DDVCS_CFF_TEST_H */
