#ifndef DVCSCROSSSECTIONDIFFERENCELUMINUS_H
#define DVCSCROSSSECTIONDIFFERENCELUMINUS_H

/**
 * @file DVCSCrossSectionDifferenceLUMinus.h
 * @author Pawel Sznajder (IPNO)
 * @date November 25, 2016
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "../../Observable.h"

/**
 * @class DVCSCrossSectionDifferenceLUMinus
 * @brief Unpolarized cross-section difference for longitudinally polarized beam and negative beam charge.
 *
 * Definition:<br>
 *
 * \f$
 * \Delta d^4\sigma_{LU}(x_{B}, t, Q^2, \phi) =
 * \frac{
 * d^4\sigma^{\rightarrow +}(x_{B}, t, Q^2, \phi) - d^4\sigma^{\leftarrow +}(x_{B}, t, Q^2, \phi)
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
class DVCSCrossSectionDifferenceLUMinus: public Observable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSCrossSectionDifferenceLUMinus(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSCrossSectionDifferenceLUMinus();

    virtual DVCSCrossSectionDifferenceLUMinus* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual double computePhiObservable(double phi);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSCrossSectionDifferenceLUMinus(
            const DVCSCrossSectionDifferenceLUMinus &other);
};

#endif /* DVCSCROSSSECTIONDIFFERENCELUMINUS_H */
