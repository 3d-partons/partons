#ifndef COMPARISON_DATA_H
#define COMPARISON_DATA_H

/**
 * @file ComparisonReportList.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 01 December 2015
 * @version 1.0
 *
 * @class ComparisonReportList
 *
 * @brief
 */

#include <string>

class ComparisonData {
public:
    ComparisonData();
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
};

#endif /* COMPARISON_DATA_H */
