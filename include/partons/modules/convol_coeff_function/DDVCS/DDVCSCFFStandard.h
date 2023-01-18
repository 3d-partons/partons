#ifndef DDVCS_CFF_STANDARD_H
#define DDVCS_CFF_STANDARD_H

#include <complex>
#include <string>
#include <vector>

#include "DDVCSConvolCoeffFunctionModule.h"

namespace PARTONS {

/**
 * @class DDVCSCFFTEST
 *
 * @brief Evaluation of DDVCS Compton form factors.
 *
 * The class implements standard way of evaluating DDVCS CFFs, which is limits are compatible with DVCS and TCS modules.
 * Only LO is available.
 */
class DDVCSCFFStandard: public DDVCSConvolCoeffFunctionModule {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     */
    DDVCSCFFStandard(const std::string &className);

    virtual DDVCSCFFStandard* clone() const;

    /**
     * Destructor.
     */
    virtual ~DDVCSCFFStandard();

    virtual void resolveObjectDependencies();

protected:

    /**
     * Copy constructor.
     */
    DDVCSCFFStandard(const DDVCSCFFStandard &other);

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
    double convolution(double x, std::vector<double> params);

    NumA::FunctionType1D* m_pConvolution; ///< Functor to convolution().

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

#endif /* DDVCS_CFF_STANDARD_H */
