#ifndef DVCS_OBSERVABLE_RESULT_H
#define DVCS_OBSERVABLE_RESULT_H

/**
 * @file DVCSObservableResult.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date September 21, 2015
 * @version 1.0
 */

#include "../../../utils/type/PhysicalType.h"
#include "../ObservableResult.h"
#include "../ObservableType.h"
#include "DVCSObservableKinematic.h"

namespace PARTONS {

/**
 * @class DVCSObservableResult
 *
 * @brief Class representing single result of DVCS observable computation.
 *
 * This class is used to store a result of a single DVCS observable computation.
 */
class DVCSObservableResult: public ObservableResult<DVCSObservableKinematic> {

public:

    /**
     * Default constructor.
     */
    DVCSObservableResult();

    /**
     * Assignment constructor.
     * @param value Value to be assigned.
     */
    DVCSObservableResult(const PhysicalType<double>& value);

    /**
     * Assignment constructor.
     * @param kinematic DVCS observable kinematic to be assigned.
     */
    DVCSObservableResult(const DVCSObservableKinematic& kinematic);

    /**
     * Assignment constructor.
     * @param type DVCS observable type to be assigned.
     */
    DVCSObservableResult(ObservableType::Type type);

    /**
     * Assignment constructor.
     * @param value Value to be assigned.
     * @param kinematic DVCS observable kinematic to be assigned.
     * @param type DVCS observable type to be assigned.
     */
    DVCSObservableResult(const PhysicalType<double>& value,
            const DVCSObservableKinematic& kinematic,
            ObservableType::Type type);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSObservableResult(const DVCSObservableResult& other);

    /**
     * Destructor.
     */
    virtual ~DVCSObservableResult();
};

} /* namespace PARTONS */

#endif /* DVCS_OBSERVABLE_RESULT_H */
