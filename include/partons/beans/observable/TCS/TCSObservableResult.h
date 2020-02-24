#ifndef TCS_OBSERVABLE_RESULT_H
#define TCS_OBSERVABLE_RESULT_H

/**
 * @file TCSObservableResult.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date September 21, 2015
 * @version 1.0
 */

#include "../../../utils/type/PhysicalType.h"
#include "../ObservableResult.h"
#include "TCSObservableKinematic.h"

namespace PARTONS {

/**
 * @class TCSObservableResult
 *
 * @brief Class representing single result of TCS observable computation.
 *
 * This class is used to store a result of a single TCS observable computation.
 */
class TCSObservableResult: public ObservableResult<TCSObservableKinematic> {

public:

    /**
     * Default constructor.
     */
    TCSObservableResult();

    /**
     * Assignment constructor.
     * @param value Value to be assigned.
     */
    TCSObservableResult(const PhysicalType<double>& value);

    /**
     * Assignment constructor.
     * @param kinematic TCS observable kinematic to be assigned.
     */
    TCSObservableResult(const TCSObservableKinematic& kinematic);

    /**
     * Assignment constructor.
     * @param value Value to be assigned.
     * @param kinematic TCS observable kinematic to be assigned.
     */
    TCSObservableResult(const PhysicalType<double>& value,
            const TCSObservableKinematic& kinematic);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    TCSObservableResult(const TCSObservableResult& other);

    /**
     * Destructor.
     */
    virtual ~TCSObservableResult();
};

} /* namespace PARTONS */

#endif /* TCS_OBSERVABLE_RESULT_H */
