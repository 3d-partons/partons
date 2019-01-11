#ifndef RESULT_H
#define RESULT_H

/**
 * @file Result.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 12 November 2015
 * @version 1.0
 */

#include <ElementaryUtils/string_utils/Formatter.h>
#include <string>

#include "channel/ChannelType.h"
#include "system/ResultInfo.h"

namespace PARTONS {

/**
 * @class Result
 *
 * @brief Base class for all result-like classes.
 *
 * This class is used as a base (abstract) class for all classes storing result information, like e.g. GPDResult or ObservableResult.
 */
template<typename KinematicType>
class Result: public BaseObject {

public:

    /**
     * Destructor.
     */
    virtual ~Result() {
    }

    virtual std::string toString() const {

        ElemUtils::Formatter formatter;

        formatter << BaseObject::toString() << " ChannelType: "
                << ChannelType(m_channelType).toString()
                << " ComputationModuleName: " << m_computationModuleName
                << "\n\n";
        formatter << "Kinematics: " << m_kinematic.toString();

        return formatter.str();
    }

    /**
     * Relation operator that checks if the value of left operand is less than the value of right operand (in this case returned is this->m_kinematic < other.m_kinematic).
     * Used by std::sort function.
     * @param other Right hand value.
     * @return True if the value of left operand is less than the value of right operand, otherwise false.
     */
    bool operator <(const Result<KinematicType> &other) const {
        return (m_kinematic < other.m_kinematic);
    }

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get channel type.
     */
    ChannelType::Type getChannelType() const {
        return m_channelType;
    }

    /**
     * Set name of module used to evaluate this result.
     */
    void setComputationModuleName(const std::string& moduleName) {
        m_computationModuleName = moduleName;
    }

    /**
     * Get name of module used to evaluate this result.
     */
    const std::string& getComputationModuleName() const {
        return m_computationModuleName;
    }

    /**
     * Set information on this result.
     */
    void setResultInfo(const ResultInfo& resultInfo) {
        m_resultInfo = resultInfo;
    }

    /**
     * Get information on this result.
     */
    const ResultInfo& getResultInfo() const {
        return m_resultInfo;
    }

    /**
     * Set kinematics associated to this result.
     */
    void setKinematic(const KinematicType& kinematic) {
        m_kinematic = kinematic;
    }

    /**
     * Get reference to kinematics associated to this result.
     */
    const KinematicType& getKinematic() const {
        return m_kinematic;
    }

protected:

    /**
     * Default constructor.
     * @param className Name of class to be associated to BaseObject (see BaseObject::m_className variable).
     * @param channelType Channel type.
     */
    Result(const std::string &className, ChannelType::Type channelType) :
            BaseObject(className), m_channelType(channelType), m_computationModuleName(
                    "UNDEFINED") {
    }

    /**
     * Assignment constructor.
     * @param kinematic Kinematics to be assigned.
     */
    Result(const std::string &className, ChannelType::Type channelType,
            const KinematicType& kinematic) :
            BaseObject(className), m_channelType(channelType), m_computationModuleName(
                    "UNDEFINED"), m_kinematic(kinematic) {
    }

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    Result(const Result &other) :
            BaseObject(other), m_channelType(other.m_channelType), m_computationModuleName(
                    other.m_computationModuleName), m_resultInfo(
                    other.m_resultInfo), m_kinematic(other.m_kinematic) {
    }

    /**
     * Channel type.
     */
    ChannelType::Type m_channelType;

    /**
     * Name of module used to evaluate this result.
     */
    std::string m_computationModuleName;

    /**
     * Information on this result.
     */
    ResultInfo m_resultInfo;

    /**
     * Kinematics associated to this result.
     */
    KinematicType m_kinematic;
};

} /* namespace PARTONS */

#endif /* RESULT_H */
