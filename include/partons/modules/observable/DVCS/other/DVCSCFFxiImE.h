#ifndef DVCSCFFXIIME_H
#define DVCSCFFXIIME_H

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
 * @class DVCSCFFxiImE
 * @brief Real part of DVCS CFF H.
 *
 * Unit: none.
 */
class DVCSCFFxiImE: public DVCSObservable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSCFFxiImE(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSCFFxiImE();

    virtual DVCSCFFxiImE* clone() const;

    virtual PhysicalType<double> computeObservable(
            const DVCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSCFFxiImE(const DVCSCFFxiImE &other);
};

} /* namespace PARTONS */

#endif /* DVCSCFFXIIME_H */
