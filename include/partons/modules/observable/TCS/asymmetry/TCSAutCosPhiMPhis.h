#ifndef TCSAUTCOSPHIMPHIS_
#define TCSAUTCOSPHIMPHIS_

/**
 * @file TCSAutCosPhiMPhis.h
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
 * @class TCSAutCosPhiMPhis
 * @brief Asymmetry probing transverse target polarization \f$\cos(\phi - \phi_{S})\f$ component.
 *
 * Unit: none.
 */
class TCSAutCosPhiMPhis: public TCSObservable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    TCSAutCosPhiMPhis(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~TCSAutCosPhiMPhis();

    virtual TCSAutCosPhiMPhis* clone() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    TCSAutCosPhiMPhis(const TCSAutCosPhiMPhis &other);

    virtual PhysicalType<double> computeObservable(
            const TCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType);
};

} /* namespace PARTONS */

#endif /* TCSAUTCOSPHIMPHIS_ */
