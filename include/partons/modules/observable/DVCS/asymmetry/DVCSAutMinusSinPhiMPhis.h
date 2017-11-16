#ifndef DVCSAUTMINUSSINPHIMPHIS_H
#define DVCSAUTMINUSSINPHIMPHIS_H

/**
 * @file DVCSAutMinusSinPhiMPhis.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 28, 2014
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "../../Observable.h"

namespace PARTONS {

/**
 * @class DVCSAutMinusSinPhiMPhis
 * @brief Transverse target asymmetry for negative beam charge.
 *
 * Definition:<br>
 *
 * \f$ \displaystyle
 * A_{UT}^{\sin\left(\phi-\phi_{S}\right)}\left(x_{B}, t, Q^2, \phi\right) =
 * \frac{
 * \mathrm{d}^4\sigma_{\downarrow}^{-}\left(x_{B}, t, Q^2, \phi\right) -
 * \mathrm{d}^4\sigma_{\uparrow}^{-}\left(x_{B}, t, Q^2, \phi\right)
 * }{
 * \mathrm{d}^4\sigma_{\downarrow}^{-}\left(x_{B}, t, Q^2, \phi\right) +
 * \mathrm{d}^4\sigma_{\uparrow}^{-}\left(x_{B}, t, Q^2, \phi\right)
 * } \, ,
 * \f$
 *
 * where
 * \f$\sigma_{t_{h}}^{b_{c}}\f$ is a single photon production cross-section (DVCS, BH and Int) for target helicity denoted by \f$t_{h}\f$ and beam charge denoted by \f$b_{c}\f$.
 */
class DVCSAutMinusSinPhiMPhis: public Observable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSAutMinusSinPhiMPhis(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAutMinusSinPhiMPhis();

    virtual DVCSAutMinusSinPhiMPhis* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual double computePhiObservable(double phi);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAutMinusSinPhiMPhis(const DVCSAutMinusSinPhiMPhis &other);
};

} /* namespace PARTONS */

#endif /* DVCSAUTMINUSSINPHIMPHIS_H */
