#ifndef JET_OBSERVABLE_RESULT_H
#define JET_OBSERVABLE_RESULT_H

/**
 * @file JETObservableResult.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date September 21, 2015
 * @version 1.0
 */

#include "../../../utils/type/PhysicalType.h"
#include "../ObservableResult.h"
#include "JETObservableKinematic.h"

namespace PARTONS {

/**
 * @class JETObservableResult
 *
 * @brief Class representing single result of JET observable computation.
 *
 * This class is used to store a result of a single JET observable computation.
 */
class JETObservableResult: public ObservableResult<JETObservableKinematic> {

public:

    /**
     * Default constructor.
     */
    JETObservableResult();

    /**
     * Assignment constructor.
     * @param value Value to be assigned.
     */
    JETObservableResult(const PhysicalType<double>& value);

    /**
     * Assignment constructor.
     * @param kinematic JET observable kinematic to be assigned.
     */
    JETObservableResult(const JETObservableKinematic& kinematic);

    /**
     * Assignment constructor.
     * @param value Value to be assigned.
     * @param kinematic JET observable kinematic to be assigned.
     */
    JETObservableResult(const PhysicalType<double>& value,
            const JETObservableKinematic& kinematic);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    JETObservableResult(const JETObservableResult& other);

    /**
     * Destructor.
     */
    virtual ~JETObservableResult();
};

} /* namespace PARTONS */

#endif /* JET_OBSERVABLE_RESULT_H */
