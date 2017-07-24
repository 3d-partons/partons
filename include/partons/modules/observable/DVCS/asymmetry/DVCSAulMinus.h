#ifndef DVCSAULMINUS
#define DVCSAULMINUS

/**
 * @file DVCSAulMinus.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 28, 2014
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "../../Observable.h"

namespace PARTONS {

/**
 * @class DVCSAulMinus
 * @brief Longitudinally polarized target asymmetry for negative beam charge.
 *
 * Definition:<br>
 *
 * \f$
 * A_{UL}(x_{B}, t, Q^2, \phi) =
 * \frac{
 * (d^4\sigma_{\leftarrow}^{\rightarrow -}(x_{B}, t, Q^2, \phi) + d^4\sigma_{\leftarrow}^{\leftarrow -}(x_{B}, t, Q^2, \phi)) - (d^4\sigma_{\rightarrow}^{\rightarrow -}(x_{B}, t, Q^2, \phi) + d^4\sigma_{\rightarrow}^{\leftarrow  -}(x_{B}, t, Q^2, \phi))
 * }{
 * (d^4\sigma_{\leftarrow}^{\rightarrow -}(x_{B}, t, Q^2, \phi) + d^4\sigma_{\leftarrow}^{\leftarrow -}(x_{B}, t, Q^2, \phi)) + (d^4\sigma_{\rightarrow}^{\rightarrow -}(x_{B}, t, Q^2, \phi) + d^4\sigma_{\rightarrow}^{\leftarrow  -}(x_{B}, t, Q^2, \phi))
 * }
 * \f$
 *
 * where:
 * \f$\sigma_{t_{h}}^{b_{h} b_{c}}\f$ is single photon production cross-section (DVCS, BH and Int) for target helicity denoted by \f$t_{h}\f$, beam helicity denoted by \f$b_{h}\f$ and beam charge denoted by \f$b_{c}\f$.
 */
class DVCSAulMinus: public Observable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSAulMinus(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAulMinus();

    virtual DVCSAulMinus* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual double computePhiObservable(double phi);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAulMinus(const DVCSAulMinus &other);
};

} /* namespace PARTONS */

#endif /* DVCSAULMINUS */
