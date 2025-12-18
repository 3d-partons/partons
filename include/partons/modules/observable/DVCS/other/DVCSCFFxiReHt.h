#ifndef DVCSCFFXIREHT_H
#define DVCSCFFXIREHT_H

/**
 * @file DVCSCFFReH.h
 * @author Pawel Sznajder (NCBJ)
 * @date December 15, 2025
 * @version 1.0
 */

#include <string>

#include "../../../../beans/gpd/GPDType.h"
#include "../../../../beans/List.h"
#include "../../../../utils/type/PhysicalType.h"
#include "../DVCSObservable.h"

namespace PARTONS {

/**
 * @class DVCSCFFxiReHt
 * @brief Real part of DVCS CFF H.
 *
 * Unit: none.
 */
class DVCSCFFxiReHt: public DVCSObservable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSCFFxiReHt(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSCFFxiReHt();

    virtual DVCSCFFxiReHt* clone() const;

    virtual PhysicalType<double> computeObservable(
            const DVCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSCFFxiReHt(const DVCSCFFxiReHt &other);
};

} /* namespace PARTONS */

#endif /* DVCSCFFXIREHT_H */
