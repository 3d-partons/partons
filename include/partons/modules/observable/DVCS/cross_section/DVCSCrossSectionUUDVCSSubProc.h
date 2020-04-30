#ifndef DVCSCROSSSECTIONUUDVCSSUBPROC_H
#define DVCSCROSSSECTIONUUDVCSSUBPROC_H

/**
 * @file DVCSCrossSectionUUDVCSSubProc.h
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
 * @class DVCSCrossSectionUUDVCSSubProc
 * @brief Unpolarized cross-section for DVCS only.
 *
 * Unit: \f$\mathrm{nbarn}/\mathrm{GeV}^4\f$.
 */
class DVCSCrossSectionUUDVCSSubProc: public DVCSObservable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSCrossSectionUUDVCSSubProc(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSCrossSectionUUDVCSSubProc();

    virtual DVCSCrossSectionUUDVCSSubProc* clone() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSCrossSectionUUDVCSSubProc(const DVCSCrossSectionUUDVCSSubProc &other);

    virtual PhysicalType<double> computeObservable(
            const DVCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType);
};

} /* namespace PARTONS */

#endif /* DVCSCROSSSECTIONUUDVCSSUBPROC_H */
