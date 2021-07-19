#ifndef DVMP_OBSERVABLE_RESULT_H
#define DVMP_OBSERVABLE_RESULT_H

/**
 * @file DVMPObservableResult.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date September 21, 2015
 * @version 1.0
 */

#include "../../../utils/type/PhysicalType.h"
#include "../ObservableResult.h"
#include "DVMPObservableKinematic.h"

namespace PARTONS {

/**
 * @class DVMPObservableResult
 *
 * @brief Class representing single result of DVMP observable computation.
 *
 * This class is used to store a result of a single DVMP observable computation.
 */
class DVMPObservableResult: public ObservableResult<DVMPObservableKinematic> {

public:

    /**
     * Default constructor.
     */
    DVMPObservableResult();

    /**
     * Assignment constructor.
     * @param value Value to be assigned.
     */
    DVMPObservableResult(const PhysicalType<double>& value);

    /**
     * Assignment constructor.
     * @param kinematic DVMP observable kinematic to be assigned.
     */
    DVMPObservableResult(const DVMPObservableKinematic& kinematic);

    /**
     * Assignment constructor.
     * @param value Value to be assigned.
     * @param kinematic DVMP observable kinematic to be assigned.
     */
    DVMPObservableResult(const PhysicalType<double>& value,
            const DVMPObservableKinematic& kinematic);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVMPObservableResult(const DVMPObservableResult& other);

    /**
     * Destructor.
     */
    virtual ~DVMPObservableResult();
};

} /* namespace PARTONS */

#endif /* DVMP_OBSERVABLE_RESULT_H */
