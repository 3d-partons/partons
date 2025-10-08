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

#include "../../utils/type/PhysicalType.h"
#include "../channel/ChannelType.h"
#include "../Result.h"

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

        formatter << "\n" << Result<KinematicType>::toString() << "\n\n";
        formatter << "Result: " << m_value.toString();
        formatter << '\n';

        return formatter.str();
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

protected:

    /**
     * Default constructor.
     */
    ObservableResult(const std::string &className,
            ChannelType::Type channelType) :
            Result<KinematicType>(className, channelType) {
    }

    /**
     * Assignment constructor.
     * @param value Value to be assigned.
     */
    ObservableResult(const std::string &className,
            ChannelType::Type channelType, const PhysicalType<double>& value) :
            Result<KinematicType>(className, channelType), m_value(value) {
    }

    /**
     * Assignment constructor.
     * @param kinematic Observable kinematics to be assigned.
     */
    ObservableResult(const std::string &className,
            ChannelType::Type channelType, const KinematicType& kinematic) :
            Result<KinematicType>(className, channelType, kinematic) {
    }

    /**
     * Assignment constructor.
     * @param value Value to be assigned.
     * @param kinematic Observable kinematics to be assigned.
     */
    ObservableResult(const std::string &className,
            ChannelType::Type channelType, const PhysicalType<double>& value,
            const KinematicType& kinematic) :
            Result<KinematicType>(className, channelType, kinematic), m_value(
                    value) {
    }

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    ObservableResult(const ObservableResult& other) :
            Result<KinematicType>(other), m_value(other.m_value) {
    }

    /**
     * Value of result.
     */
    PhysicalType<double> m_value;
};

} /* namespace PARTONS */

#endif /* OBSERVABLE_RESULT_H */
