#ifndef DVCSALUDVCSSIN1PHI_H
#define DVCSALUDVCSSIN1PHI_H

/**
 * @file DVCSAluDVCSSin1Phi.h
 * @author Luca COLANERI (IPNO)
 * @date July 19, 2016
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>

#include "../../../MathIntegratorModule.h"
#include "DVCSAluDVCS.h"

namespace PARTONS {

/**
 * @class DVCSAluDVCSSin1Phi
 *
 * @brief 1th Fourier moment of longitudinally polarized beam asymmetry for sum over beam charges (DVCS part).
 *
 * Definition:<br>
 *
 * \f$
 * A_{LU, DVCS}^{\sin(1\phi)}(x_{B}, t, Q^2) =
 * \frac{1}{\pi}
 * \int_{0}^{2\pi} d\phi A_{LU, DVCS}(x_{B}, t, Q^2, \phi) \sin(1\phi)
 * \f$
 *
 * where \f$A_{LU, DVCS}(x_{B}, t, Q^2, \phi)\f$ is defined in DVCSAluDVCS.
 */
class DVCSAluDVCSSin1Phi: public DVCSAluDVCS, public MathIntegratorModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSAluDVCSSin1Phi(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAluDVCSSin1Phi();

    virtual DVCSAluDVCSSin1Phi* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual double computeFourierObservable();

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAluDVCSSin1Phi(const DVCSAluDVCSSin1Phi &other);

    /**
     * Functor to perform the integration.
     */
    NumA::FunctionType1D* m_pFunctionToIntegrateObservable;

    /**
     * Function to be integrated.
     */
    virtual double functionToIntegrateObservable(double x,
            std::vector<double> params);

    /**
     * Initialize functors.
     */
    void initFunctorsForIntegrations();
};

} /* namespace PARTONS */

#endif /* DVCSALUDVCSSIN1PHI_H */
