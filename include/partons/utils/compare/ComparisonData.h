#ifndef COMPARISON_DATA_H
#define COMPARISON_DATA_H

/**
 * @file ComparisonData.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 24 February 2016
 * @version 1.0
 *
 * @class ComparisonData
 *
 * @brief
 */

#include <NumA/utils/Differences.h>
#include <string>

class ComparisonData {
public:
    ComparisonData(bool isPassed, const std::string& variableName,
            const std::string& variableValue,
            const std::string& variableValueReference,
            const NumA::Differences &differences,
            const std::string &objectInfo);

    virtual ~ComparisonData();

    std::string toString() const;

    // #############################
    // ##### GETTERS & SETTERS #####
    // #############################

    bool isIsComparisonPassed() const;
    void setIsComparisonPassed(bool isComparisonPassed);

private:
    bool m_isComparisonPassed;

    std::string m_objectInfo;

    std::string m_variableName;
    std::string m_variableValue;
    std::string m_variableValueReference;

    NumA::Differences m_differences;
};

#endif /* COMPARISON_DATA_H */
