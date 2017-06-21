#ifndef DVCSALUINT_H
#define DVCSALUINT_H

/**
 * @file DVCSAluInt.h
 * @author Luca COLANERI (IPNO)
 * @date July 19, 2016
 * @version 1.0
 */

#include <string>

#include "../../Observable.h"

/**
 * @class DVCSAluInt
 * @brief Longitudinally polarized beam asymmetry for difference over beam charges (interference part).
 *
 * Definition:<br>
 *
 * \f$
 * A_{LU, Int}(x_{B}, t, Q^2, \phi) =
 * \frac{
 * (d^4\sigma^{\rightarrow +}(x_{B}, t, Q^2, \phi) - d^4\sigma^{\rightarrow -}(x_{B}, t, Q^2, \phi)) - (d^4\sigma^{\leftarrow +}(x_{B}, t, Q^2, \phi) - d^4\sigma^{\leftarrow -}(x_{B}, t, Q^2, \phi))
 * }{
 * (d^4\sigma^{\rightarrow +}(x_{B}, t, Q^2, \phi) - d^4\sigma^{\rightarrow -}(x_{B}, t, Q^2, \phi)) + (d^4\sigma^{\leftarrow +}(x_{B}, t, Q^2, \phi) - d^4\sigma^{\leftarrow -}(x_{B}, t, Q^2, \phi))
 * }
 * \f$
 *
 * where:
 * \f$\sigma^{b_{h} b_{c}}\f$ is single photon production cross-section (BH and Int) for beam helicity denoted by \f$b_{h}\f$ and beam charge denoted by \f$b_{c}\f$.
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
    virtual double computePhiObservable(double phi);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAluInt(const DVCSAluInt &other);
};

#endif /* DVCSALUINT_H */
