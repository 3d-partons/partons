#ifndef DVCSALUPLUS_H
#define DVCSALUPLUS_H

/**
 * @file DVCSAluPlus.h
 * @author Pawel Sznajder (NCBJ, Warsaw)
 * @date November 16, 2017
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "../../Observable.h"

namespace PARTONS {

/**
 * @class DVCSAluPlus
 * @brief Longitudinally polarized beam asymmetry for positive beam charge.
 *
 * Definition:<br>
 *
 * \f$ \displaystyle
 * A_{LU}\left(x_{B}, t, Q^2, \phi\right) =
 * \frac{
 * \mathrm{d}^4\sigma^{\rightarrow +}\left(x_{B}, t, Q^2, \phi\right) -
 * \mathrm{d}^4\sigma^{\leftarrow +}\left(x_{B}, t, Q^2, \phi\right)
 * }{
 * \mathrm{d}^4\sigma^{\rightarrow +}\left(x_{B}, t, Q^2, \phi\right) +
 * \mathrm{d}^4\sigma^{\leftarrow +}\left(x_{B}, t, Q^2, \phi\right)
 * } \, ,
 * \f$
 *
 * where
 * \f$\sigma^{b_{h} b_{c}}\f$ is a single photon production cross-section (DVCS, BH and Int) for beam helicity denoted by \f$b_{h}\f$ and beam charge denoted by \f$b_{c}\f$.
 */
class DVCSAluPlus: public Observable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSAluPlus(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAluPlus();

    virtual DVCSAluPlus* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual double computePhiObservable(double phi);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAluPlus(const DVCSAluPlus &other);
};

} /* namespace PARTONS */

#endif /* DVCSALUPLUS_H */
