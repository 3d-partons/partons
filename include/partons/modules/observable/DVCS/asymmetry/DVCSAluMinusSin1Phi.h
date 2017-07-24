#ifndef DVCSALUMINUSSIN1PHI_H
#define DVCSALUMINUSSIN1PHI_H

/**
 * @file DVCSAluMinusSin1Phi.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date September 28, 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>

#include "../../../MathIntegratorModule.h"
#include "DVCSAluMinus.h"

/**
 * @class DVCSAluMinusSin1Phi
 *
 * @brief 1th Fourier moment of longitudinally polarized beam asymmetry for negative beam charge.
 *
 * Definition:<br>
 *
 * \f$
 * A_{LU}^{\sin(1\phi)}(x_{B}, t, Q^2) =
 * \frac{1}{\pi}
 * \int_{0}^{2\pi} d\phi A_{LU}(x_{B}, t, Q^2, \phi) \sin(1\phi)
 * \f$
 *
 * where \f$A_{LU}(x_{B}, t, Q^2, \phi)\f$ is defined in DVCSAluMinus.
 */
class DVCSAluMinusSin1Phi: public DVCSAluMinus, public MathIntegratorModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSAluMinusSin1Phi(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAluMinusSin1Phi();

    virtual DVCSAluMinusSin1Phi* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual double computeFourierObservable();

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAluMinusSin1Phi(const DVCSAluMinusSin1Phi &other);

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

#endif /* DVCSALUMINUSSIN1PHI_H */
