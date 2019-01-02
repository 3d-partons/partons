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

#include "../../utils/compare/ComparisonReport.h"
#include "../channel/ChannelType.h"
#include "../Result.h"
#include "ObservableType.h"

namespace PARTONS {

/**
 * @class ObservableResult
 *
 * @brief Abstract class representing single observable result.
 *
 * This abstract class is used to store results of a single observable computation.
 */
class ObservableResult: public Result {

public:

    /**
     * Default constructor.
     */
    ObservableResult(const std::string &className,
            ChannelType::Type channelType);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    ObservableResult(const ObservableResult& other);

    /**
     * Destructor.
     */
    virtual ~ObservableResult();

    virtual std::string toString() const;

    /**
     * Set value.
     * @param value Value to be set.
     * @param observableType Type of observable.
     */
    void set(double value, ObservableType::Type observableType);

    /**
     * Compare to other DVCSConvolCoeffFunctionResult object and store comparison result in given comparison report.
     * @param rootComparisonReport Reference to comparison report to be used to store comparison result.
     * @param referenceObject Reference to object to be compared.
     * @param parentObjectInfo Addition information coming from the parent object (if needed).
     */
    void compare(ComparisonReport &rootComparisonReport,
            const ObservableResult &referenceObject,
            std::string parentObjectInfo = ElemUtils::StringUtils::EMPTY) const;

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get value of result.
     */
    double getValue() const;

    /**
     * Set value of result.
     */
    void setValue(double value);

    /**
     * Get type of observable associated to this result.
     */
    ObservableType::Type getObservableType() const;

    /**
     * Set type of observable associated to this result.
     */
    void setObservableType(ObservableType::Type observableType);

private:

    /**
     * Value of result.
     */
    double m_value;

    /**
     * Type of observable associated to this result.
     */
    ObservableType::Type m_observableType;
};

} /* namespace PARTONS */

#endif /* OBSERVABLE_RESULT_H */
