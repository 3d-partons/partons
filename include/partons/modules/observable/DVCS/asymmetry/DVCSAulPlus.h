#ifndef DVCSAULPLUS
#define DVCSAULPLUS

/**
 * @file DVCSAulPlus.h
 * @author Pawel Sznajder (NCBJ, Warsaw)
 * @date November 16, 2017
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "../../Observable.h"

namespace PARTONS {

/**
 * @class DVCSAulPlus
 * @brief Longitudinally polarized target asymmetry for positive beam charge.
 *
 * Definition:<br>
 *
 * \f$ \displaystyle
 * A_{UL}\left(x_{B}, t, Q^2, \phi\right) =
 * \frac{
 * \mathrm{d}^4\sigma_{\leftarrow}^{+}\left(x_{B}, t, Q^2, \phi\right) -
 * \mathrm{d}^4\sigma_{\rightarrow}^{+}\left(x_{B}, t, Q^2, \phi\right)
 * }{
 * \mathrm{d}^4\sigma_{\leftarrow}^{+}\left(x_{B}, t, Q^2, \phi\right) +
 * \mathrm{d}^4\sigma_{\rightarrow}^{+}\left(x_{B}, t, Q^2, \phi\right)
 * } \, ,
 * \f$
 *
 * where
 * \f$\sigma_{t_{h}}^{b_{c}}\f$ is q single photon production cross-section (DVCS, BH and Interference) for target helicity denoted by \f$t_{h}\f$ and beam charge denoted by \f$b_{c}\f$.
 */
class DVCSAulPlus: public Observable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSAulPlus(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAulPlus();

    virtual DVCSAulPlus* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual double computePhiObservable(double phi);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAulPlus(const DVCSAulPlus &other);
};

} /* namespace PARTONS */

#endif /* DVCSAULPLUS */
