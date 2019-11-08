#ifndef TCSAUTSINPHIMPHIS_
#define TCSAUTSINPHIMPHIS_

/**
 * @file TCSAutSinPhiMPhis.h
 * @author Pawel Sznajder
 * @author Oskar Grocholski
 * @date November 8, 2019
 * @version 1.0
 */

#include <string>

#include "../../../../beans/gpd/GPDType.h"
#include "../../../../beans/List.h"
#include "../../../../utils/type/PhysicalType.h"
#include "../TCSObservable.h"

namespace PARTONS {

/**
 * @class TCSAutSinPhiMPhis
 * @brief Asymmetry probing transverse target polarization \f$\sin(\phi - \phi_{S})\f$ component.
 *
 * Unit: none.
 */
class TCSAutSinPhiMPhis: public TCSObservable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    TCSAutSinPhiMPhis(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~TCSAutSinPhiMPhis();

    virtual TCSAutSinPhiMPhis* clone() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    TCSAutSinPhiMPhis(const TCSAutSinPhiMPhis &other);

    virtual PhysicalType<double> computeObservable(
            const TCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType);
};

} /* namespace PARTONS */

#endif /* TCSAUTSINPHIMPHIS_ */
