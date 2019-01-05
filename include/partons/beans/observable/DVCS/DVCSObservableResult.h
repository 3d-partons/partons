#ifndef DVCS_OBSERVABLE_RESULT_H
#define DVCS_OBSERVABLE_RESULT_H

/**
 * @file DVCSObservableResult.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date September 21, 2015
 * @version 1.0
 */

#include "../ObservableResult.h"
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
     * @param kinematic DVCS observable kinematic to be assigned.
     */
    DVCSObservableResult(const DVCSObservableKinematic& kinematic);

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
