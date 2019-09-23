#ifndef DVCSALUINTSIN1Phi_H
#define DVCSALUINTSIN1Phi_H

/**
 * @file DVCSAluIntSin1Phi.h
 * @author Luca COLANERI (IPNO)
 * @date July 19, 2016
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>

#include "../../../../beans/gpd/GPDType.h"
#include "../../../../beans/List.h"
#include "../../../../utils/type/PhysicalType.h"
#include "../../../MathIntegratorModule.h"
#include "DVCSAluInt.h"

namespace PARTONS {

/**
 * @class DVCSAluIntSin1Phi
 *
 * @brief 1th Fourier moment of longitudinally polarized beam asymmetry for difference over beam charges (interference part).
 *
 * Definition:<br>
 *
 * \f$ \displaystyle
 * A_{LU, Int}^{\sin\left(1\phi\right)}\left(x_{B}, t, Q^2\right) =
 * \frac{1}{\pi}
 * \int_{0}^{2\pi} \mathrm{d}\phi A_{LU, Int}\left(x_{B}, t, Q^2, \phi\right) \sin\left(1\phi\right) \, ,
 * \f$
 *
 * where \f$A_{LU, Int}\left(x_{B}, t, Q^2, \phi\right)\f$ is defined in DVCSAluInt.
 */
class DVCSAluIntSin1Phi: public DVCSAluInt, public MathIntegratorModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSAluIntSin1Phi(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAluIntSin1Phi();

    virtual DVCSAluIntSin1Phi* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAluIntSin1Phi(const DVCSAluIntSin1Phi &other);

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

#endif /* DVCSALUINTSIN1Phi_H */
