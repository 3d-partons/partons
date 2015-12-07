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

class ComparisonData {
public:
    ComparisonData();
    ComparisonData(const std::string & variableName);
    ComparisonData(const std::string & variableName,
            const std::string & additionalInfo);
    virtual ~ComparisonData();

    std::string toString() const;

    void setAdditionalInfo(const std::string& additionalInfo);
    void setVariableName(const std::string& variableName);

    const std::string& getAdditionalInfo() const;
    const std::string& getVariableName() const;

private:
    std::string m_variableName;
    std::string m_additionalInfo;

    double m_absoluteDifference;
    double m_relativeDifference;
    double m_absoluteTolerance;
    double m_relativeTolerance;
};

#endif /* COMPARISON_DATA_H */
