#ifndef DVCSAUTDVCSSINPHIMPHIS_H
#define DVCSAUTDVCSSINPHIMPHIS_H

/**
 * @file DVCSAutDVCSSinPhiMPhis.h
 * @author Pawel Sznajder (NCBJ, Warsaw)
 * @date November 16, 2017
 * @version 1.0
 */

#include <string>

#include "../../../../beans/gpd/GPDType.h"
#include "../../../../beans/List.h"
#include "../../../../utils/type/PhysicalType.h"
#include "../DVCSObservable.h"

namespace PARTONS {

/**
 * @class DVCSAutDVCSSinPhiMPhis
 * @brief Transverse target asymmetry (DVCS part).
 *
 * Definition:<br>
 *
 * \f$ \displaystyle
 * A_{UT, DVCS}^{\sin\left(\phi-\phi_{S}\right)}\left(x_{B}, t, Q^2, \phi\right) =
 * \frac{
 * \left(\mathrm{d}^4\sigma_{\downarrow}^{+}\left(x_{B}, t, Q^2, \phi\right) +
 * \mathrm{d}^4\sigma_{\downarrow}^{-}\left(x_{B}, t, Q^2, \phi\right)\right) -
 * \left(\mathrm{d}^4\sigma_{\uparrow}^{+}\left(x_{B}, t, Q^2, \phi\right) +
 * \mathrm{d}^4\sigma_{\uparrow}^{-}\left(x_{B}, t, Q^2, \phi\right)\right)
 * }{
 * \left(\mathrm{d}^4\sigma_{\downarrow}^{+}\left(x_{B}, t, Q^2, \phi\right) +
 * \mathrm{d}^4\sigma_{\downarrow}^{-}\left(x_{B}, t, Q^2, \phi\right)\right) +
 * \left(\mathrm{d}^4\sigma_{\uparrow}^{+}\left(x_{B}, t, Q^2, \phi\right) +
 * \mathrm{d}^4\sigma_{\uparrow}^{-}\left(x_{B}, t, Q^2, \phi\right)\right)
 * } \, ,
 * \f$
 *
 * where
 * \f$\sigma_{t_{h}}^{b_{c}}\f$ is a single photon production cross-section (DVCS, BH and Int) for target helicity denoted by \f$t_{h}\f$ and beam charge denoted by \f$b_{c}\f$.
 */
class DVCSAutDVCSSinPhiMPhis: public DVCSObservable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSAutDVCSSinPhiMPhis(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSAutDVCSSinPhiMPhis();

    virtual DVCSAutDVCSSinPhiMPhis* clone() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSAutDVCSSinPhiMPhis(const DVCSAutDVCSSinPhiMPhis &other);

    virtual PhysicalType<double> computeObservable(
            const DVCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType);
};

} /* namespace PARTONS */

#endif /* DVCSAUTDVCSSINPHIMPHIS_H */
