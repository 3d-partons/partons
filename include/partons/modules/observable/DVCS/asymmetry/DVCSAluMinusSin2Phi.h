#ifndef DVCSALUMINUSSIN2PHI_H
#define DVCSALUMINUSSIN2PHI_H

/**
 * @file DVCSAluMinusSin2Phi.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date September 28, 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>

#include "../../../../beans/gpd/GPDType.h"
#include "../../../../beans/List.h"
#include "../../../../utils/type/PhysicalType.h"
#include "../../../MathIntegratorModule.h"
#include "DVCSAluMinus.h"

namespace PARTONS {

/**
 * @class DVCSAluMinusSin2Phi
 *
 * @brief 1th Fourier moment of longitudinally polarized beam asymmetry for negative beam charge.
 *
 * Definition:<br>
 *
 * \f$ \displaystyle
 * A_{LU}^{\sin\left(2\phi\right)}\left(x_{B}, t, Q^2\right) =
 * \frac{1}{\pi}
 * \int_{0}^{2\pi} \mathrm{d}\phi A_{LU}\left(x_{B}, t, Q^2, \phi\right) \sin\left(2\phi\right) \, ,
 * \f$
 *
 * where \f$A_{LU}\left(x_{B}, t, Q^2, \phi\right)\f$ is defined in DVCSAluMinus.
 */
class DVCSAluMinusSin2Phi: public DVCSAluMinus, public MathIntegratorModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSAluMinusSin2Phi(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAluMinusSin2Phi();

    virtual DVCSAluMinusSin2Phi* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAluMinusSin2Phi(const DVCSAluMinusSin2Phi &other);

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

#endif /* DVCSALUMINUSSIN2PHI_H */
