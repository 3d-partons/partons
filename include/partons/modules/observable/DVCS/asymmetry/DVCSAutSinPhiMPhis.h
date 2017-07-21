#ifndef DVCSAUTSINPHIMPHIS_H
#define DVCSAUTSINPHIMPHIS_H

/**
 * @file DVCSAutSinPhiMPhis.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 28, 2014
 * @version 1.0
 */

#include <string>

#include "../../Observable.h"

/**
 * @class DVCSAutSinPhiMPhis
 * @brief Transverse target beam asymmetry for negative beam charge.
 *
 * Definition:<br>
 *
 * \f$
 * A_{UT}^{\sin(\phi-\phi_{S})}(x_{B}, t, Q^2, \phi) =
 * \frac{
 * (d^4\sigma_{\downarrow}^{\rightarrow -}(x_{B}, t, Q^2, \phi) + d^4\sigma_{\downarrow}^{\rightarrow -}(x_{B}, t, Q^2, \phi)) - (d^4\sigma_{\uparrow}^{\leftarrow -}(x_{B}, t, Q^2, \phi) + d^4\sigma_{\uparrow}^{\leftarrow -}(x_{B}, t, Q^2, \phi))
 * }{
 * (d^4\sigma_{\downarrow}^{\rightarrow -}(x_{B}, t, Q^2, \phi) + d^4\sigma_{\downarrow}^{\rightarrow -}(x_{B}, t, Q^2, \phi)) + (d^4\sigma_{\uparrow}^{\leftarrow -}(x_{B}, t, Q^2, \phi) + d^4\sigma_{\uparrow}^{\leftarrow -}(x_{B}, t, Q^2, \phi))
 * }
 * \f$
 *
 * where:
 * \f$\sigma_{t_{h}}^{b_{h} b_{c}}\f$ is single photon production cross-section (DVCS, BH and Int) for target helicity denoted by \f$t_{h}\f$, beam helicity denoted by \f$b_{h}\f$ and beam charge denoted by \f$b_{c}\f$.
 */
class DVCSAutSinPhiMPhis: public Observable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSAutSinPhiMPhis(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAutSinPhiMPhis();

    virtual DVCSAutSinPhiMPhis* clone() const;
    virtual double computePhiObservable(double phi);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAutSinPhiMPhis(const DVCSAutSinPhiMPhis &other);
};

#endif /* DVCSAUTSINPHIMPHIS_H */
