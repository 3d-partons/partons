#ifndef DVCSAULMINUS
#define DVCSAULMINUS

/**
 * @file DVCSAulMinus.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 28, 2014
 * @version 1.0
 */

#include <string>

#include "../../../../beans/gpd/GPDType.h"
#include "../../../../beans/List.h"
#include "../../../../utils/type/PhysicalType.h"
#include "../DVCSObservable.h"

namespace PARTONS {

/**
 * @class DVCSAulMinus
 * @brief Longitudinally polarized target asymmetry for negative beam charge.
 *
 * Definition:<br>
 *
 * \f$ \displaystyle
 * A_{UL}\left(x_{B}, t, Q^2, \phi\right) =
 * \frac{
 * \mathrm{d}^4\sigma_{\leftarrow}^{-}\left(x_{B}, t, Q^2, \phi\right) -
 * \mathrm{d}^4\sigma_{\rightarrow}^{-}\left(x_{B}, t, Q^2, \phi\right)
 * }{
 * \mathrm{d}^4\sigma_{\leftarrow}^{-}\left(x_{B}, t, Q^2, \phi\right) +
 * \mathrm{d}^4\sigma_{\rightarrow}^{-}\left(x_{B}, t, Q^2, \phi\right)
 * } \, ,
 * \f$
 *
 * where
 * \f$\sigma_{t_{h}}^{b_{c}}\f$ is q single photon production cross-section (DVCS, BH and Interference) for target helicity denoted by \f$t_{h}\f$ and beam charge denoted by \f$b_{c}\f$.
 */
class DVCSAulMinus: public DVCSObservable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSAulMinus(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAulMinus();

    virtual DVCSAulMinus* clone() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAulMinus(const DVCSAulMinus &other);

    virtual PhysicalType<double> computeObservable(
            const DVCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType);
};

} /* namespace PARTONS */

#endif /* DVCSAULMINUS */
