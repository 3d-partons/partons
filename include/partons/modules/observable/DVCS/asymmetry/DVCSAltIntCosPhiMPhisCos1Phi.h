#ifndef DVCSALTINTCOSPHIMPHISCOS1PHI_H
#define DVCSALTINTCOSPHIMPHISCOS1PHI_H

/**
 * @file DVCSAltIntCosPhiMPhisCos1Phi.h
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
#include "DVCSAltIntCosPhiMPhis.h"

namespace PARTONS {

/**
 * @class DVCSAltIntCosPhiMPhisCos1Phi
 *
 * @brief 1th Fourier moment of longitudinal beam transverse target asymmetry (interference part).
 *
 * Definition:<br>
 *
 * \f$ \displaystyle
 * A_{LT, Int}^{\cos\left(\phi-\phi_{S}\right)\cos\left(1\phi\right)}\left(x_{B}, t, Q^2\right) =
 * \frac{1}{\pi}
 * \int_{0}^{2\pi} \mathrm{d}\phi A_{LT, Int}^{\cos\left(\phi-\phi_{S}\right)}\left(x_{B}, t, Q^2, \phi\right) \cos\left(1\phi\right) \, ,
 * \f$
 *
 * where \f$A_{LT, Int}^{\cos\left(\phi-\phi_{S}\right)}\left(x_{B}, t, Q^2, \phi\right)\f$ is defined in DVCSAltIntCosPhiMPhis.
 */
class DVCSAltIntCosPhiMPhisCos1Phi: public DVCSAltIntCosPhiMPhis,
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
    DVCSAltIntCosPhiMPhisCos1Phi(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAltIntCosPhiMPhisCos1Phi();

    virtual DVCSAltIntCosPhiMPhisCos1Phi* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAltIntCosPhiMPhisCos1Phi(const DVCSAltIntCosPhiMPhisCos1Phi &other);

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

#endif /* DVCSALTINTCOSPHIMPHISCOS1PHI_H */
