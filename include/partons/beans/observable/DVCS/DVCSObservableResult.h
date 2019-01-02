#ifndef DVCS_OBSERVABLE_RESULT_H
#define DVCS_OBSERVABLE_RESULT_H

/**
 * @file DVCSObservableResult.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date September 21, 2015
 * @version 1.0
 */

#include <string>

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
class DVCSObservableResult: public ObservableResult {

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

    virtual std::string toString() const;

    /**
     * Relation operator that checks if the value of left operand is less than the value of right operand (in this case returned is this->m_kinematic < other.m_kinematic).
     * Used by std::sort function.
     * @param other Right hand value.
     * @return True if the value of left operand is less than the value of right operand, otherwise false.
     */
    bool operator <(const DVCSObservableResult &other) const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get reference to DVCS observable kinematics associated to this result.
     */
    const DVCSObservableKinematic& getKinematic() const;

    /**
     * Set DVCS observable kinematics associated to this result.
     */
    void setKinematic(const DVCSObservableKinematic &kinematic);

private:

    /**
     * DVCS observable kinematics associated to this result.
     */
    DVCSObservableKinematic m_kinematic;
};

} /* namespace PARTONS */

#endif /* DVCS_OBSERVABLE_RESULT_H */
