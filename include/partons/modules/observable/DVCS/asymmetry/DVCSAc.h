#ifndef DVCSAC_H
#define DVCSAC_H

/**
 * @file DVCSAc.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 25, 2014
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "../../Observable.h"

namespace PARTONS {

/**
 * @class DVCSAc
 * @brief Beam charge asymmetry.
 *
 * Definition:<br>
 *
 * \f$ \displaystyle
 * A_{C}\left(x_{B}, t, Q^2, \phi\right) =
 * \frac{
 * \left(\mathrm{d}^4\sigma^{\rightarrow +}\left(x_{B}, t, Q^2, \phi\right) +
 * \mathrm{d}^4\sigma^{\leftarrow +}\left(x_{B}, t, Q^2, \phi\right)\right) -
 * \left(\mathrm{d}^4\sigma^{\rightarrow -}\left(x_{B}, t, Q^2, \phi\right) +
 * \mathrm{d}^4\sigma^{\leftarrow -}\left(x_{B}, t, Q^2, \phi\right)\right)
 * }{
 * \left(\mathrm{d}^4\sigma^{\rightarrow +}\left(x_{B}, t, Q^2, \phi\right) +
 * \mathrm{d}^4\sigma^{\leftarrow +}\left(x_{B}, t, Q^2, \phi\right)\right) +
 * \left(\mathrm{d}^4\sigma^{\rightarrow -}\left(x_{B}, t, Q^2, \phi\right) +
 * \mathrm{d}^4\sigma^{\leftarrow -}\left(x_{B}, t, Q^2, \phi\right)\right)
 * } \, ,
 * \f$
 *
 * where
 * \f$\sigma^{b_{h} b_{c}}\f$ is a single photon production cross-section (DVCS, BH and Interference) for beam helicity denoted by \f$b_{h}\f$ and beam charge denoted by \f$b_{c}\f$.
 */
class DVCSAc: public Observable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSAc(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAc();

    virtual DVCSAc* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual double computePhiObservable(double phi);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAc(const DVCSAc &other);
};

} /* namespace PARTONS */

#endif /* DVCSAC_H */
