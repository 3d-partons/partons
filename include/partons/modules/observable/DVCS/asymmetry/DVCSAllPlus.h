#ifndef DVCSALLPLUS_H
#define DVCSALLPLUS_H

/**
 * @file DVCSAllPlus.h
 * @author Pawel Sznajder (NCBJ, Warsaw)
 * @date November 16, 2017
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "../../Observable.h"

namespace PARTONS {

/**
 * @class DVCSAllPlus
 * @brief Longitudinally polarized beam and target asymmetry for positive beam charge.
 *
 * Definition:<br>
 *
 * \f$ \displaystyle
 * A_{LL}\left(x_{B}, t, Q^2, \phi\right) =
 * \frac{
 * \left(\mathrm{d}^4\sigma_{\leftarrow}^{\rightarrow +}\left(x_{B}, t, Q^2, \phi\right) +
 * \mathrm{d}^4\sigma_{\rightarrow}^{\leftarrow +}\left(x_{B}, t, Q^2, \phi\right)\right) -
 * \left(\mathrm{d}^4\sigma_{\rightarrow}^{\rightarrow +}\left(x_{B}, t, Q^2, \phi\right) +
 * \mathrm{d}^4\sigma_{\leftarrow}^{\leftarrow +}\left(x_{B}, t, Q^2, \phi\right)\right)
 * }{
 * \left(\mathrm{d}^4\sigma_{\leftarrow}^{\rightarrow +}\left(x_{B}, t, Q^2, \phi\right) +
 * \mathrm{d}^4\sigma_{\rightarrow}^{\leftarrow +}\left(x_{B}, t, Q^2, \phi\right)\right) +
 * \left(\mathrm{d}^4\sigma_{\rightarrow}^{\rightarrow +}\left(x_{B}, t, Q^2, \phi\right) +
 * \mathrm{d}^4\sigma_{\leftarrow}^{\leftarrow +}\left(x_{B}, t, Q^2, \phi\right)\right)
 * } \, ,
 * \f$
 *
 * where
 * \f$\sigma_{t_{h}}^{b_{h} b_{c}}\f$ is a single photon production cross-section (DVCS, BH and Interference) for target helicity denoted by \f$t_{h}\f$, beam helicity denoted by \f$b_{h}\f$ and beam charge denoted by \f$b_{c}\f$.
 */
class DVCSAllPlus: public Observable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSAllPlus(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAllPlus();

    virtual DVCSAllPlus* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual double computePhiObservable(double phi);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAllPlus(const DVCSAllPlus &other);
};

} /* namespace PARTONS */

#endif /* DVCSALLPLUS_H */
