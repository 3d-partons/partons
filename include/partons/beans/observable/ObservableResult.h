#ifndef OBSERVABLE_RESULT_H
#define OBSERVABLE_RESULT_H

/**
 * @file ObservableResult.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date September 21, 2015
 * @version 1.0
 */

#include <ElementaryUtils/string_utils/StringUtils.h>
#include <string>

#include "../../utils/math/ErrorBar.h"
#include "../Result.h"
#include "ObservableKinematic.h"
#include "ObservableType.h"

class ComparisonReport;
namespace NumA {
class Tolerances;
} /* namespace NumA */

/**
 * @class ObservableResult
 *
 * @brief
 */
class ObservableResult: public Result {
public:
    static const std::string PARAMETER_NAME_OBSERVABLE_VALUE;
    static const std::string PARAMETER_NAME_TOTAL_ERROR;

    ObservableResult();
    ObservableResult(const std::string &observableName, double value);
    virtual ~ObservableResult();

    virtual std::string toString() const;
    virtual std::string getObjectInfo() const;

    void compare(ComparisonReport &rootComparisonReport,
            const ObservableResult &referenceObject,
            const NumA::Tolerances &tolerances, std::string parentObjectInfo =
                    ElemUtils::StringUtils::EMPTY) const;

    void setKinematic(const ObservableKinematic &kinematic);
    void setStatError(const ErrorBar& statError);
    void setSystError(const ErrorBar& systError);
    void setTotalError(double totalError);
    void setValue(double value);
    void setObservableType(ObservableType::Type observableType);

    const ObservableKinematic& getKinematic() const;
    ObservableType::Type getObservableType() const;
    const std::string& getObservableName() const;
    const ErrorBar& getStatError() const;
    const ErrorBar& getSystError() const;
    double getTotalError() const;
    double getValue() const;

private:
    std::string m_observableName;

    double m_value;
    double m_totalError;

    ErrorBar m_statError;
    ErrorBar m_systError;

    //TODO add a proxy to retrieve it from database.
    ObservableKinematic m_kinematic;

    ObservableType::Type m_observableType;
};

#endif /* OBSERVABLE_RESULT_H */
