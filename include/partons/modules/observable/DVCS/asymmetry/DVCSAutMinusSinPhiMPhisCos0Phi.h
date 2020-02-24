#ifndef DVCSAUTMINUSSINPHIMPHISCOS0PHI_H
#define DVCSAUTMINUSSINPHIMPHISCOS0PHI_H

/**
 * @file DVCSAutMinusSinPhiMPhisCos0Phi.h
 * @author Luca COLANERI (IPNO)
 * @date July 18, 2016
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>

#include "../../../../beans/gpd/GPDType.h"
#include "../../../../beans/List.h"
#include "../../../../utils/type/PhysicalType.h"
#include "../../../MathIntegratorModule.h"
#include "DVCSAutMinusSinPhiMPhis.h"

namespace PARTONS {
class DVCSObservableKinematic;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class DVCSAutMinusSinPhiMPhisCos0Phi
 *
 * @brief 0th Fourier moment of transverse target asymmetry for negative beam charge.
 *
 * Definition:<br>
 *
 * \f$ \displaystyle
 * A_{UT}^{\sin\left(\phi-\phi_{S}\right)\cos\left(0\phi\right)}\left(x_{B}, t, Q^2\right) =
 * \frac{1}{2\pi}
 * \int_{0}^{2\pi} \mathrm{d}\phi A_{UT}^{\sin\left(\phi-\phi_{S}\right)}\left(x_{B}, t, Q^2, \phi\right) \cos\left(0\phi\right) \, ,
 * \f$
 *
 * where \f$A_{UT}^{\sin\left(\phi-\phi_{S}\right)}\left(x_{B}, t, Q^2, \phi\right)\f$ is defined in DVCSAutMinusSinPhiMPhis.
 */
class DVCSAutMinusSinPhiMPhisCos0Phi: public DVCSAutMinusSinPhiMPhis,
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
    DVCSAutMinusSinPhiMPhisCos0Phi(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAutMinusSinPhiMPhisCos0Phi();

    virtual DVCSAutMinusSinPhiMPhisCos0Phi* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAutMinusSinPhiMPhisCos0Phi(const DVCSAutMinusSinPhiMPhisCos0Phi &other);

    virtual PhysicalType<double> computeObservable(
            const DVCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType);

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

#endif /* DVCSAUTMINUSSINPHIMPHISCOS0PHI_H */
