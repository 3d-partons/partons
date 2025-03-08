#ifndef DDVCS_OBSERVABLE_RESULT_H
#define DDVCS_OBSERVABLE_RESULT_H

/**
 * @file DDVCSObservableResult.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date September 21, 2015
 * @version 1.0
 */

#include "../../../utils/type/PhysicalType.h"
#include "../ObservableResult.h"
#include "DDVCSObservableKinematic.h"

namespace PARTONS {

/**
 * @class DDVCSObservableResult
 *
 * @brief Class representing single result of DDVCS observable computation.
 *
 * This class is used to store a result of a single DDVCS observable computation.
 */
class DDVCSObservableResult: public ObservableResult<DDVCSObservableKinematic> {

public:

    /**
     * Default constructor.
     */
    DDVCSObservableResult();

    /**
     * Assignment constructor.
     * @param value Value to be assigned.
     */
    DDVCSObservableResult(const PhysicalType<double>& value);

    /**
     * Assignment constructor.
     * @param kinematic DDVCS observable kinematic to be assigned.
     */
    DDVCSObservableResult(const DDVCSObservableKinematic& kinematic);

    /**
     * Assignment constructor.
     * @param value Value to be assigned.
     * @param kinematic DDVCS observable kinematic to be assigned.
     */
    DDVCSObservableResult(const PhysicalType<double>& value,
            const DDVCSObservableKinematic& kinematic);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DDVCSObservableResult(const DDVCSObservableResult& other);

    /**
     * Destructor.
     */
    virtual ~DDVCSObservableResult();
};

} /* namespace PARTONS */

#endif /* DDVCS_OBSERVABLE_RESULT_H */
