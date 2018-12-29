#ifndef DVCS_CONVOL_COEFF_FUNCTION_RESULT_H
#define DVCS_CONVOL_COEFF_FUNCTION_RESULT_H

/**
 * @file DVCSConvolCoeffFunctionResult.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 22 July 2015
 * @version 1.0
 */

#include <string>

#include "../ConvolCoeffFunctionResult.h"
#include "DVCSConvolCoeffFunctionKinematic.h"

namespace PARTONS {

/**
 * @class DVCSConvolCoeffFunctionResult
 *
 * @brief Class representing single result of DVCS CFF computation.
 *
 * This class is used to store results of a single DVCS CCF computation.
 */
class DVCSConvolCoeffFunctionResult: public ConvolCoeffFunctionResult {

public:

    /**
     * Default constructor.
     */
    DVCSConvolCoeffFunctionResult();

    /**
     * Assignment constructor.
     * @param kinematic DVCS CCF kinematics to be assigned.
     */
    DVCSConvolCoeffFunctionResult(
            const DVCSConvolCoeffFunctionKinematic& kinematic);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSConvolCoeffFunctionResult(const DVCSConvolCoeffFunctionResult& other);

    /**
     * Destructor.
     */
    virtual ~DVCSConvolCoeffFunctionResult();

    virtual std::string toString() const;

    /**
     * Relation operator that checks if the value of left operand is less than the value of right operand (in this case returned is this->m_kinematic < other.m_kinematic).
     * Used by std::sort function.
     * @param other Right hand value.
     * @return True if the value of left operand is less than the value of right operand, otherwise false.
     */
    bool operator <(const DVCSConvolCoeffFunctionResult &other) const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get reference to DVCS CFF kinematics associated to this result.
     */
    const DVCSConvolCoeffFunctionKinematic& getKinematic() const;

    /**
     * Set DVCS CFF kinematics associated to this result.
     */
    void setKinematic(const DVCSConvolCoeffFunctionKinematic& kinematic);

private:

    /**
     * DVCS CCF kinematics associated to this result.
     */
    DVCSConvolCoeffFunctionKinematic m_kinematic;
};

} /* namespace PARTONS */

#endif /* DVCS_CONVOL_COEFF_FUNCTION_RESULT_H */
