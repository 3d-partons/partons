#ifndef DVCSALUDVCS_H
#define DVCSALUDVCS_H

/**
 * @file DVCSAluDVCS.h
 * @author Luca COLANERI (IPNO)
 * @date July 19, 2016
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "../../Observable.h"

namespace PARTONS {

/**
 * @class DVCSAluDVCS
 * @brief Longitudinally polarized beam asymmetry for sum over beam charges (DVCS part).
 *
 * Definition:<br>
 *
 * \f$
 * A_{LU, DVCS}(x_{B}, t, Q^2, \phi) =
 * \frac{
 * (d^4\sigma^{\rightarrow +}(x_{B}, t, Q^2, \phi) + d^4\sigma^{\rightarrow -}(x_{B}, t, Q^2, \phi)) - (d^4\sigma^{\leftarrow +}(x_{B}, t, Q^2, \phi) + d^4\sigma^{\leftarrow -}(x_{B}, t, Q^2, \phi))
 * }{
 * (d^4\sigma^{\rightarrow +}(x_{B}, t, Q^2, \phi) + d^4\sigma^{\rightarrow -}(x_{B}, t, Q^2, \phi)) + (d^4\sigma^{\leftarrow +}(x_{B}, t, Q^2, \phi) + d^4\sigma^{\leftarrow -}(x_{B}, t, Q^2, \phi))
 * }
 * \f$
 *
 * where:
 * \f$\sigma^{b_{h} b_{c}}\f$ is single photon production cross-section (BH and Int) for beam helicity denoted by \f$b_{h}\f$ and beam charge denoted by \f$b_{c}\f$.
 */
class DVCSAluDVCS: public Observable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSAluDVCS(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAluDVCS();

    virtual DVCSAluDVCS* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual double computePhiObservable(double phi);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAluDVCS(const DVCSAluDVCS &other);
};

} /* namespace PARTONS */

#endif /* DVCSALUDVCS_H */
