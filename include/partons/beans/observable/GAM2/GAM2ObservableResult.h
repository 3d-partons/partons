#ifndef GAM2_OBSERVABLE_RESULT_H
#define GAM2_OBSERVABLE_RESULT_H

/**
 * @file GAM2ObservableResult.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date September 21, 2015
 * @version 1.0
 */

#include "../../../utils/type/PhysicalType.h"
#include "../ObservableResult.h"
#include "GAM2ObservableKinematic.h"

namespace PARTONS {

/**
 * @class GAM2ObservableResult
 *
 * @brief Class representing single result of two photon production observable computation.
 *
 * This class is used to store a result of a single two photon production observable computation.
 */
class GAM2ObservableResult: public ObservableResult<GAM2ObservableKinematic> {

public:

    /**
     * Default constructor.
     */
    GAM2ObservableResult();

    /**
     * Assignment constructor.
     * @param value Value to be assigned.
     */
    GAM2ObservableResult(const PhysicalType<double>& value);

    /**
     * Assignment constructor.
     * @param kinematic GAM2 observable kinematic to be assigned.
     */
    GAM2ObservableResult(const GAM2ObservableKinematic& kinematic);

    /**
     * Assignment constructor.
     * @param value Value to be assigned.
     * @param kinematic GAM2 observable kinematic to be assigned.
     */
    GAM2ObservableResult(const PhysicalType<double>& value,
            const GAM2ObservableKinematic& kinematic);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    GAM2ObservableResult(const GAM2ObservableResult& other);

    /**
     * Destructor.
     */
    virtual ~GAM2ObservableResult();
};

} /* namespace PARTONS */

#endif /* GAM2_OBSERVABLE_RESULT_H */
