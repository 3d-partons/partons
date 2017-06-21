#ifndef DVCSCROSSSECTIONUUMINUS_H
#define DVCSCROSSSECTIONUUMINUS_H

/**
 * @file DVCSCrossSectionUUMinus.h
 * @author Pawel Sznajder (IPNO)
 * @date November 25, 2016
 * @version 1.0
 */

#include <string>

#include "../../Observable.h"

/**
 * @class DVCSCrossSectionUUMinus
 * @brief Unpolarized cross-section for negative beam charge.
 *
 * Definition:<br>
 *
 * \f$
 * d^4\sigma_{UU}(x_{B}, t, Q^2, \phi) =
 * \frac{
 * d^4\sigma^{\rightarrow +}(x_{B}, t, Q^2, \phi) + d^4\sigma^{\leftarrow +}(x_{B}, t, Q^2, \phi)
 * }{
 * 2
 * }
 * \f$
 *
 * where:
 * \f$\sigma^{b_{h} b_{c}}\f$ is single photon production cross-section (DVCS, BH and Int) for beam helicity denoted by \f$b_{h}\f$ and beam charge denoted by \f$b_{c}\f$.
 *
 * Units:<br>
 *
 *\f$\mathrm{nbarn}/\mathrm{GeV}^4\f$
 */
class DVCSCrossSectionUUMinus: public Observable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSCrossSectionUUMinus(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSCrossSectionUUMinus();

    virtual DVCSCrossSectionUUMinus* clone() const;
    virtual double computePhiObservable(double phi);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSCrossSectionUUMinus(const DVCSCrossSectionUUMinus &other);
};

#endif /* DVCSCROSSSECTIONUUMINUS_H */
