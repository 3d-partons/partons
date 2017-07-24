#ifndef DVCSALUMINUS_H
#define DVCSALUMINUS_H

/**
 * @file DVCSAluMinus.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 28, 2014
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "../../Observable.h"

/**
 * @class DVCSAluMinus
 * @brief Longitudinally polarized beam asymmetry for negative beam charge.
 *
 * Definition:<br>
 *
 * \f$
 * A_{LU}(x_{B}, t, Q^2, \phi) =
 * \frac{
 * d^4\sigma^{\rightarrow -}(x_{B}, t, Q^2, \phi) - d^4\sigma^{\leftarrow -}(x_{B}, t, Q^2, \phi)
 * }{
 * d^4\sigma^{\rightarrow -}(x_{B}, t, Q^2, \phi) + d^4\sigma^{\leftarrow -}(x_{B}, t, Q^2, \phi)
 * }
 * \f$
 *
 * where:
 * \f$\sigma^{b_{h} b_{c}}\f$ is single photon production cross-section (BH and Int) for beam helicity denoted by \f$b_{h}\f$ and beam charge denoted by \f$b_{c}\f$.
 */
class DVCSAluMinus: public Observable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSAluMinus(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAluMinus();

    virtual DVCSAluMinus* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual double computePhiObservable(double phi);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAluMinus(const DVCSAluMinus &other);
};

#endif /* DVCSALUMINUS_H */
