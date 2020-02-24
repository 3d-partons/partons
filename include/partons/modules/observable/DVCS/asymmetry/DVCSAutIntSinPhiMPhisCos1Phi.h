#ifndef DVCSAUTINTSINPHIMPHISCOS1PHI_H
#define DVCSAUTINTSINPHIMPHISCOS1PHI_H

/**
 * @file DVCSAutIntSinPhiMPhisCos1Phi.h
 * @author Pawel Sznajder (NCBJ, Warsaw)
 * @date November 16, 2017
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>

#include "../../../../beans/gpd/GPDType.h"
#include "../../../../beans/List.h"
#include "../../../../utils/type/PhysicalType.h"
#include "../../../MathIntegratorModule.h"
#include "DVCSAutIntSinPhiMPhis.h"

namespace PARTONS {

/**
 * @class DVCSAutIntSinPhiMPhisCos1Phi
 *
 * @brief 1th Fourier moment of transverse target asymmetry (interference part).
 *
 * Definition:<br>
 *
 * \f$ \displaystyle
 * A_{UT, Int}^{\sin\left(\phi-\phi_{S}\right)\cos\left(1\phi\right)}\left(x_{B}, t, Q^2\right) =
 * \frac{1}{\pi}
 * \int_{0}^{2\pi} \mathrm{d}\phi A_{UT}^{\sin\left(\phi-\phi_{S}\right)}\left(x_{B}, t, Q^2, \phi\right) \cos\left(1\phi\right) \, ,
 * \f$
 *
 * where \f$A_{UT, Int}^{\sin\left(\phi-\phi_{S}\right)}\left(x_{B}, t, Q^2, \phi\right)\f$ is defined in DVCSAutIntSinPhiMPhis.
 */
class DVCSAutIntSinPhiMPhisCos1Phi: public DVCSAutIntSinPhiMPhis,
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
    DVCSAutIntSinPhiMPhisCos1Phi(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAutIntSinPhiMPhisCos1Phi();

    virtual DVCSAutIntSinPhiMPhisCos1Phi* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAutIntSinPhiMPhisCos1Phi(const DVCSAutIntSinPhiMPhisCos1Phi &other);

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

#endif /* DVCSAUTINTSINPHIMPHISCOS1PHI_H */
