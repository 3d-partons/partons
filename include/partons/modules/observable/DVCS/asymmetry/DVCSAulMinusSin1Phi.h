#ifndef DVCSAULMINUSSIN1PHI
#define DVCSAULMINUSSIN1PHI

/**
 * @file DVCSAulMinusSin1Phi.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date September 28, 2015
 * @version 1.0
 */

#include <string>
#include <vector>

#include "../../../MathIntegratorModule.h"
#include "DVCSAulMinus.h"

/**
 * @class DVCSAulMinusSin1Phi
 *
 * @brief 1th Fourier moment of longitudinally polarized target asymmetry for negative beam charge.
 *
 * Definition:<br>
 *
 * \f$
 * A_{UL}^{\sin(1\phi)}(x_{B}, t, Q^2) =
 * \frac{1}{\pi}
 * \int_{0}^{2\pi} d\phi A_{UL}(x_{B}, t, Q^2, \phi) \sin(1\phi)
 * \f$
 *
 * where \f$A_{UL}(x_{B}, t, Q^2, \phi)\f$ is defined in DVCSAulMinus.
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

#endif /* DVCSAULMINUSSIN1PHI */
