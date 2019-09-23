#ifndef DVCSAULPLUSSIN2PHI
#define DVCSAULPLUSSIN2PHI

/**
 * @file DVCSAulPlusSin2Phi.h
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
#include "DVCSAulPlus.h"

namespace PARTONS {

/**
 * @class DVCSAulPlusSin2Phi
 *
 * @brief 2th Fourier moment of longitudinally polarized target asymmetry for positive beam charge.
 *
 * Definition:<br>
 *
 * \f$ \displaystyle
 * A_{UL}^{\sin\left(2\phi\right)}\left(x_{B}, t, Q^2\right) =
 * \frac{1}{\pi}
 * \int_{0}^{2\pi} \mathrm{d}\phi A_{UL}\left(x_{B}, t, Q^2, \phi\right) \sin\left(2\phi\right) \, ,
 * \f$
 *
 * where \f$A_{UL}\left(x_{B}, t, Q^2, \phi\right)\f$ is defined in DVCSAulPlus.
 */
class DVCSAulPlusSin2Phi: public DVCSAulPlus, public MathIntegratorModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSAulPlusSin2Phi(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAulPlusSin2Phi();

    virtual DVCSAulPlusSin2Phi* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAulPlusSin2Phi(const DVCSAulPlusSin2Phi &other);

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

#endif /* DVCSAULPLUSSIN2PHI */
