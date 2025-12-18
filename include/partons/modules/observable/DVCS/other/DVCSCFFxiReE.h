#ifndef DVCSCFFXIREE_H
#define DVCSCFFXIREE_H

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
 * @class DVCSCFFxiReE
 * @brief Real part of DVCS CFF H.
 *
 * Unit: none.
 */
class DVCSCFFxiReE: public DVCSObservable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSCFFxiReE(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSCFFxiReE();

    virtual DVCSCFFxiReE* clone() const;

    virtual PhysicalType<double> computeObservable(
            const DVCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSCFFxiReE(const DVCSCFFxiReE &other);
};

} /* namespace PARTONS */

#endif /* DVCSCFFXIREE_H */
