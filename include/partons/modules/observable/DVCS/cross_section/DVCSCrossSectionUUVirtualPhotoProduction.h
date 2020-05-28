#ifndef DVCSCROSSSECTIONUUVIRTUALPHOTOPRODUCTION_H
#define DVCSCROSSSECTIONUUVIRTUALPHOTOPRODUCTION_H

/**
 * @file DVCSCrossSectionUUVirtualPhotoProduction.h
 * @author Pawel Sznajder (IPNO)
 * @date November 25, 2016
 * @version 1.0
 */

#include <string>

#include "../../../../beans/gpd/GPDType.h"
#include "../../../../beans/List.h"
#include "../../../../utils/type/PhysicalType.h"
#include "../DVCSObservable.h"

namespace PARTONS {

/**
 * @class DVCSCrossSectionUUVirtualPhotoProduction
 * @brief Unpolarized cross-section for virtual photo-production.
 *
 * Unit: \f$\mathrm{nbarn}/\mathrm{GeV}^4\f$.
 */
class DVCSCrossSectionUUVirtualPhotoProduction: public DVCSObservable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSCrossSectionUUVirtualPhotoProduction(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSCrossSectionUUVirtualPhotoProduction();

    virtual DVCSCrossSectionUUVirtualPhotoProduction* clone() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSCrossSectionUUVirtualPhotoProduction(
            const DVCSCrossSectionUUVirtualPhotoProduction &other);

    virtual PhysicalType<double> computeObservable(
            const DVCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType);

private:

    /**
     * Virtual-photon flux.
     */
    double getVirtualPhotonFlux(const DVCSObservableKinematic& kinematic) const;
};

} /* namespace PARTONS */

#endif /* DVCSCROSSSECTIONUUVIRTUALPHOTOPRODUCTION_H */
