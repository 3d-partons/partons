#ifndef GPD_RESULT_H
#define GPD_RESULT_H

/**
 * @file GPDSubtractionConstantResult.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 26 April 2015
 * @version 1.0
 */

#include <string>

#include "../../utils/compare/ComparisonReport.h"
#include "../../utils/type/PhysicalType.h"
#include "../Result.h"
#include "GPDSubtractionConstantKinematic.h"

namespace PARTONS {

/**
 * @class GPDSubtractionConstantResult
 *
 * @brief Class representing single result of GPD subtraction constant computation.
 *
 * This class is used to store results of a single GPD subtraction constant computation.
 */
class GPDSubtractionConstantResult: public Result<
        GPDSubtractionConstantKinematic> {

public:

    /**
     * Default constructor.
     */
    GPDSubtractionConstantResult();

    /**
     * Assignment constructor.
     * @param value Value to be assigned.
     */
    GPDSubtractionConstantResult(const PhysicalType<double>& value);

    /**
     * Assignment constructor.
     * @param kinematic GPD kinematics to be assigned.
     */
    GPDSubtractionConstantResult(
            const GPDSubtractionConstantKinematic& kinematic);

    /**
     * Assignment constructor.
     * @param value Value to be assigned.
     * @param kinematic GPD kinematics to be assigned.
     */
    GPDSubtractionConstantResult(const PhysicalType<double>& value,
            const GPDSubtractionConstantKinematic& kinematic);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    GPDSubtractionConstantResult(const GPDSubtractionConstantResult &other);

    /**
     * Destructor.
     */
    virtual ~GPDSubtractionConstantResult();

    virtual std::string toString() const;

    /**
     * Compare to other GPDSubtractionConstantResult object and store comparison result in given comparison report.
     * @param rootComparisonReport Reference to comparison report to be used to store comparison result.
     * @param referenceObject Reference to object to be compared.
     * @param parentObjectInfo Addition information coming from the parent object (if needed).
     */
    void compare(ComparisonReport &rootComparisonReport,
            const GPDSubtractionConstantResult &referenceObject,
            std::string parentObjectInfo = "") const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get value of result.
     */
    const PhysicalType<double>& getValue() const;

    /**
     * Set value of result.
     */
    void setValue(const PhysicalType<double>& value);

private:

    /**
     * Result of computation.
     */
    PhysicalType<double> m_value;
};

} /* namespace PARTONS */

#endif /* GPD_RESULT_H */
