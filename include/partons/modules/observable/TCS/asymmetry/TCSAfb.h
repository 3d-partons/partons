#ifndef TCSACU_H
#define TCSACU_H

/**
 * @file TCSAfb.h
 * @author Pawel Sznajder (IPNO)
 * @date November 25, 2016
 * @version 1.0
 */

#include <string>

#include "../../../../beans/gpd/GPDType.h"
#include "../../../../beans/List.h"
#include "../../../../utils/type/PhysicalType.h"
#include "../TCSObservable.h"

namespace PARTONS {

/**
 * @class TCSAfb
 * @brief Forward-backward TCS asymmetry.
 *
 * Unit: none.
 */
class TCSAfb: public TCSObservable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    TCSAfb(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~TCSAfb();

    virtual TCSAfb* clone() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    TCSAfb(const TCSAfb &other);

    virtual PhysicalType<double> computeObservable(
            const TCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType);
};

} /* namespace PARTONS */

#endif /* TCSACU_H */
