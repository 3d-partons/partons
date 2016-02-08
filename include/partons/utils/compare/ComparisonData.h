#ifndef COMPARISON_DATA_H
#define COMPARISON_DATA_H

/**
 * @file ComparisonData.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 01 December 2015
 * @version 1.0
 *
 * @class ComparisonData
 *
 * @brief
 */

#include <string>

class Differences;
class Tolerances;

class ComparisonData {
public:
    ComparisonData(bool isPassed, const std::string &variableName,
            const std::string &variableValue,
            const std::string &variableValueReference);

    ComparisonData(bool isPassed, const std::string& variableName,
            const std::string& variableValue,
            const std::string& variableValueReference, Tolerances* pTolerances,
            Differences* pDifferences);

    virtual ~ComparisonData();

    std::string toString() const;

    void setAdditionalInfo(const std::string& additionalInfo);
    void setVariableName(const std::string& variableName);

    const std::string& getAdditionalInfo() const;
    const std::string& getVariableName() const;

private:
    bool m_passed;

    std::string m_variableName;
    std::string m_variableValue;
    std::string m_variableValueReference;

    // TODO Useful ?
    std::string m_additionalInfo;

    Tolerances* m_pTolerances;
    Differences* m_pDifferences;
};

#endif /* COMPARISON_DATA_H */
