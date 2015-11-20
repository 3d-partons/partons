#ifndef OBSERVABLE_RESULT_H
#define OBSERVABLE_RESULT_H

/**
 * @file ObservableResult.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 21 September 2015
 * @version 1.0
 *
 * @class ObservableResult
 *
 * @brief
 */

#include <string>

#include "../../utils/math/ErrorBar.h"
#include "../Result.h"
#include "ObservableKinematic.h"
#include "ObservableType.h"

class ObservableResult: public Result {
public:
    ObservableResult();
    ObservableResult(const std::string &observableName, double value);
    ObservableResult(const std::string &observableName, double phi,
            double value);
    virtual ~ObservableResult();

    virtual std::string toString();

    void setPhi(double phi);
    void setKinematic(const ObservableKinematic &kinematic);
    void setStatError(const ErrorBar& statError);
    void setSystError(const ErrorBar& systError);
    void setTotalError(double totalError);
    void setValue(double value);
    void setObservableType(ObservableType::Type observableType);

    const ObservableKinematic& getKinematic() const;
    ObservableType::Type getObservableType() const;
    const std::string& getObservableName() const;
    double getPhi() const;
    const ErrorBar& getStatError() const;
    const ErrorBar& getSystError() const;
    double getTotalError() const;
    double getValue() const;

private:
    std::string m_observableName;

    double m_value;
    double m_phi;
    double m_totalError;

    ErrorBar m_statError;
    ErrorBar m_systError;

    ObservableKinematic m_kinematic;

    ObservableType::Type m_observableType;
};

#endif /* OBSERVABLE_RESULT_H */
