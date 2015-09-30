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
#include "ObservableKinematic.h"

class ObservableResult {
public:
    ObservableResult();
    ObservableResult(double value);
    ObservableResult(double phi, double value);
    virtual ~ObservableResult();

    virtual std::string toString();

    void setPhi(double phi);
    void setKinematic(const ObservableKinematic &kinematic);
    void setStatError(const ErrorBar& statError);
    void setSystError(const ErrorBar& systError);
    void setTotalError(double totalError);
    void setValue(double value);

private:
    double m_value;
    double m_phi;
    double m_totalError;

    ErrorBar m_statError;
    ErrorBar m_systError;

    ObservableKinematic m_kinematic;
};

#endif /* OBSERVABLE_RESULT_H */
