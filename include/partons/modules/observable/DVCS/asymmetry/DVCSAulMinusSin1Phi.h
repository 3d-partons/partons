#ifndef DVCSAULMINUSSIN1PHI
#define DVCSAULMINUSSIN1PHI

/**
 * @file DVCSAulMinusSin1Phi.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date September 28, 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>

#include "../../../MathIntegratorModule.h"
#include "DVCSAulMinus.h"

namespace PARTONS {

/**
 * @class DVCSAulMinusSin1Phi
 *
 * @brief 1th Fourier moment of longitudinally polarized target asymmetry for negative beam charge.
 *
 * Definition:<br>
 *
 * \f$ \displaystyle
 * A_{UL}^{\sin\left(1\phi\right)}\left(x_{B}, t, Q^2\right) =
 * \frac{1}{\pi}
 * \int_{0}^{2\pi} \mathrm{d}\phi A_{UL}\left(x_{B}, t, Q^2, \phi\right) \sin\left(1\phi\right) \, ,
 * \f$
 *
 * where \f$A_{UL}\left(x_{B}, t, Q^2, \phi\right)\f$ is defined in DVCSAulMinus.
 */
class DVCSAulMinusSin1Phi: public DVCSAulMinus, public MathIntegratorModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSAulMinusSin1Phi(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAulMinusSin1Phi();

    virtual DVCSAulMinusSin1Phi* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual double computeFourierObservable();

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAulMinusSin1Phi(const DVCSAulMinusSin1Phi &other);

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

#endif /* DVCSAULMINUSSIN1PHI */
