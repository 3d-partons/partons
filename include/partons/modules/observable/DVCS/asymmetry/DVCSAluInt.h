#ifndef DVCSALUINT_H
#define DVCSALUINT_H

/**
 * @file DVCSAluInt.h
 * @author Luca COLANERI (IPNO)
 * @date July 19, 2016
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "../../Observable.h"

namespace PARTONS {

/**
 * @class DVCSAluInt
 * @brief Longitudinally polarized beam asymmetry for difference over beam charges (interference part).
 *
 * Definition:<br>
 *
 * \f$ \displaystyle
 * A_{LU, Int}\left(x_{B}, t, Q^2, \phi\right) =
 * \frac{
 * \left(\mathrm{d}^4\sigma^{\rightarrow +}\left(x_{B}, t, Q^2, \phi\right) -
 * \mathrm{d}^4\sigma^{\rightarrow -}\left(x_{B}, t, Q^2, \phi\right)\right) -
 * \left(\mathrm{d}^4\sigma^{\leftarrow +}\left(x_{B}, t, Q^2, \phi\right) -
 * \mathrm{d}^4\sigma^{\leftarrow -}\left(x_{B}, t, Q^2, \phi\right)\right)
 * }{
 * \left(\mathrm{d}^4\sigma^{\rightarrow +}\left(x_{B}, t, Q^2, \phi\right) -
 * \mathrm{d}^4\sigma^{\rightarrow -}\left(x_{B}, t, Q^2, \phi\right)\right) +
 * \left(\mathrm{d}^4\sigma^{\leftarrow +}\left(x_{B}, t, Q^2, \phi\right) -
 * \mathrm{d}^4\sigma^{\leftarrow -}\left(x_{B}, t, Q^2, \phi\right)\right)
 * } \, ,
 * \f$
 *
 * where
 * \f$\sigma^{b_{h} b_{c}}\f$ is a single photon production cross-section (DVCS, BH and Interference) for beam helicity denoted by \f$b_{h}\f$ and beam charge denoted by \f$b_{c}\f$.
 */
class DVCSAluInt: public Observable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSAluInt(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAluInt();

    virtual DVCSAluInt* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual double computePhiObservable(double phi);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAluInt(const DVCSAluInt &other);
};

} /* namespace PARTONS */

#endif /* DVCSALUINT_H */
