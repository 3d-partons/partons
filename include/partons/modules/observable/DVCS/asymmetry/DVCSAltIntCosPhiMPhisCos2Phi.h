#ifndef DVCSALTINTCOSPHIMPHISCOS2PHI_H
#define DVCSALTINTCOSPHIMPHISCOS2PHI_H

/**
 * @file DVCSAltIntCosPhiMPhisCos2Phi.h
 * @author Pawel Sznajder (NCBJ, Warsaw)
 * @date November 16, 2017
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>

#include "../../../MathIntegratorModule.h"
#include "DVCSAltIntCosPhiMPhis.h"

namespace PARTONS {

/**
 * @class DVCSAltIntCosPhiMPhisCos2Phi
 *
 * @brief 2th Fourier moment of longitudinal beam transverse target asymmetry (interference part).
 *
 * Definition:<br>
 *
 * \f$ \displaystyle
 * A_{LT, Int}^{\cos\left(\phi-\phi_{S}\right)\cos\left(2\phi\right)}\left(x_{B}, t, Q^2\right) =
 * \frac{1}{\pi}
 * \int_{0}^{2\pi} \mathrm{d}\phi A_{LT, Int}^{\cos\left(\phi-\phi_{S}\right)}\left(x_{B}, t, Q^2, \phi\right) \cos\left(2\phi\right) \, ,
 * \f$
 *
 * where \f$A_{LT, Int}^{\cos\left(\phi-\phi_{S}\right)}\left(x_{B}, t, Q^2, \phi\right)\f$ is defined in DVCSAltIntCosPhiMPhis.
 */
class DVCSAltIntCosPhiMPhisCos2Phi: public DVCSAltIntCosPhiMPhis,
        public MathIntegratorModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSAltIntCosPhiMPhisCos2Phi(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAltIntCosPhiMPhisCos2Phi();

    virtual DVCSAltIntCosPhiMPhisCos2Phi* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual double computeFourierObservable();

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAltIntCosPhiMPhisCos2Phi(const DVCSAltIntCosPhiMPhisCos2Phi &other);

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

#endif /* DVCSALTINTCOSPHIMPHISCOS2PHI_H */
