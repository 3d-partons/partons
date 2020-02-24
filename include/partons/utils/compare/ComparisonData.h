#ifndef COMPARISON_DATA_H
#define COMPARISON_DATA_H

/**
 * @file ComparisonData.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 24 February 2016
 * @version 1.0
 */

#include <NumA/utils/Differences.h>
#include <string>

namespace PARTONS {
/**
 * @class ComparisonData
 *
 * @brief Comparison report for single data point.
 *
 * This class stores a comparison result for a single data point.
 */
class ComparisonData {

public:

    /**
     * Constructor.
     * @param isPassed Boolean value indicating if the comparison is passed.
     * @param variableName Name of variable.
     * @param variableValue Value of variable.
     * @param variableValueReference Reference value of variable.
     * @param differences Difference.
     * @param objectInfo Information on object being compared.
     */
    ComparisonData(bool isPassed, const std::string& variableName,
            const std::string& variableValue,
            const std::string& variableValueReference,
            const NumA::Differences &differences,
            const std::string &objectInfo);

    /**
     * Destructor.
     */
    virtual ~ComparisonData();

    /**
     * Return a pre-formatted characters string for output visualization of class member's values.
     * @return Pre-formatted characters string.
     */
    std::string toString() const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Check if the comparison is passed.
     */
    bool isComparisonPassed() const;

    /**
     * Set if the comparison is passed.
     */
    void setComparisonPassed(bool isComparisonPassed);

    /**
     * Get variable name.
     */
    const std::string& getVariableName() const;

    /**
     * Set variable name.
     */
    void setVariableName(const std::string& variableName);

    /**
     * Get variable value.
     */
    const std::string& getVariableValue() const;

    /**
     * Set variable value.
     */
    void setVariableValue(const std::string& variableValue);

    /**
     * Get variable value reference.
     */
    const std::string& getVariableValueReference() const;

    /**
     * Set variable value reference.
     */
    void setVariableValueReference(const std::string& variableValueReference);

    /**
     * Get differences.
     */
    const NumA::Differences& getDifferences() const;

    /**
     * Set differences.
     */
    void setDifferences(const NumA::Differences& differences);

    /**
     * Get object info.
     */
    const std::string& getObjectInfo() const;

    /**
     * Set object info.
     */
    void setObjectInfo(const std::string& objectInfo);

private:

    /**
     * Boolean value indicating if the comparison is passed.
     */
    bool m_isComparisonPassed;

    /**
     * Name of variable.
     */
    std::string m_variableName;

    /**
     * Value of variable.
     */
    std::string m_variableValue;

    /**
     * Reference value of variable.
     */
    std::string m_variableValueReference;

    /**
     * Difference.
     */
    NumA::Differences m_differences;

    /**
     * Information on object being compared.
     */
    std::string m_objectInfo;
};

} /* namespace PARTONS */

#endif /* COMPARISON_DATA_H */
