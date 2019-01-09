#ifndef OBSERVABLE_RESULT_H
#define OBSERVABLE_RESULT_H

/**
 * @file ObservableResult.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date September 21, 2015
 * @version 1.0
 */

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <string>

#include "../../utils/compare/CompareUtils.h"
#include "../../utils/compare/ComparisonData.h"
#include "../../utils/compare/ComparisonReport.h"
#include "../../utils/type/PhysicalType.h"
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
template<typename KinematicType>
class ObservableResult: public Result<KinematicType> {

public:

    /**
     * Destructor.
     */
    virtual ~ObservableResult() {
    }

    virtual std::string toString() const {

        ElemUtils::Formatter formatter;

        formatter << Result<KinematicType>::toString();

        formatter << "\tOservable: " << m_value.toString() << " type: "
                << ObservableType(m_observableType).toString() << '\n';

        return formatter.str();
    }

    /**
     * Set value.
     * @param value Value to be set.
     * @param observableType Type of observable.
     */
    void setResult(const PhysicalType<double>& value,
            ObservableType::Type observableType) {

        m_value = value;
        m_observableType = observableType;
    }

    /**
     * Compare to other DVCSConvolCoeffFunctionResult object and store comparison result in given comparison report.
     * @param rootComparisonReport Reference to comparison report to be used to store comparison result.
     * @param referenceObject Reference to object to be compared.
     * @param parentObjectInfo Addition information coming from the parent object (if needed).
     */
    void compare(ComparisonReport &rootComparisonReport,
            const ObservableResult &referenceObject,
            std::string parentObjectInfo = ElemUtils::StringUtils::EMPTY) const {

        //TODO faire un test pour valider la cinématique associée

        if (m_observableType != referenceObject.getObservableType()) {
            throw ElemUtils::CustomException(this->getClassName(), __func__,
                    "Cannot compare objects, they are different (different name or type)");
        }

        ComparisonData xb_comparisonData = CompareUtils::compareDouble(
                "observable value", getValue(), referenceObject.getValue(),
                rootComparisonReport.getTolerances(),
                ElemUtils::Formatter() << parentObjectInfo
                        << this->getResultInfo().toString());
        //    << this->getObjectInfo());
        rootComparisonReport.addComparisonData(xb_comparisonData);
    }

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get value of result.
     */
    const PhysicalType<double>& getValue() const {
        return m_value;
    }

    /**
     * Set value of result.
     */
    void setValue(const PhysicalType<double>& value) {
        m_value = value;
    }

    /**
     * Get type of observable associated to this result.
     */
    ObservableType::Type getObservableType() const {
        return m_observableType;
    }

    /**
     * Set type of observable associated to this result.
     */
    void setObservableType(ObservableType::Type observableType) {
        m_observableType = observableType;
    }

protected:

    /**
     * Default constructor.
     */
    ObservableResult(const std::string &className,
            ChannelType::Type channelType) :
            Result<KinematicType>(className, channelType), m_observableType(
                    ObservableType::UNDEFINED) {
    }

    /**
     * Assignment constructor.
     * @param value Value to be assigned.
     */
    ObservableResult(const std::string &className,
            ChannelType::Type channelType, const PhysicalType<double>& value) :
            Result<KinematicType>(className, channelType), m_value(value), m_observableType(
                    ObservableType::UNDEFINED) {
    }

    /**
     * Assignment constructor.
     * @param kinematic Observable kinematics to be assigned.
     */
    ObservableResult(const std::string &className,
            ChannelType::Type channelType, const KinematicType& kinematic) :
            Result<KinematicType>(className, channelType, kinematic), m_observableType(
                    ObservableType::UNDEFINED) {
    }

    /**
     * Assignment constructor.
     * @param type Observable type to be assigned.
     */
    ObservableResult(const std::string &className,
            ChannelType::Type channelType, ObservableType::Type type) :
            Result<KinematicType>(className, channelType), m_observableType(
                    type) {
    }

    /**
     * Assignment constructor.
     * @param value Value to be assigned.
     * @param kinematic Observable kinematics to be assigned.
     * @param type Observable type to be assigned.
     */
    ObservableResult(const std::string &className,
            ChannelType::Type channelType, const PhysicalType<double>& value,
            const KinematicType& kinematic, ObservableType::Type type) :
            Result<KinematicType>(className, channelType, kinematic), m_value(
                    value), m_observableType(type) {
    }

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    ObservableResult(const ObservableResult& other) :
            Result<KinematicType>(other), m_value(other.m_value), m_observableType(
                    other.m_observableType) {
    }

    /**
     * Value of result.
     */
    PhysicalType<double> m_value;

    /**
     * Type of observable associated to this result.
     */
    ObservableType::Type m_observableType;
};

} /* namespace PARTONS */

#endif /* OBSERVABLE_RESULT_H */
