#ifndef OBSERVABLE_KINEMATIC_H
#define OBSERVABLE_KINEMATIC_H

/**
 * @file ObservableKinematic.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 26 November 2014
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>

#include "../channel/ChannelType.h"
#include "../Kinematic.h"

namespace PARTONS {

/**
 * @class ObservableKinematic
 *
 * @brief Abstract class representing single observable kinematics.
 *
 * This class represents a single observable kinematics (none).
 */
class ObservableKinematic: public Kinematic {

public:

    /**
     * Destructor.
     */
    virtual ~ObservableKinematic();

    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual std::string toString() const;

    /**
     * Serialize into given Packet.
     * @param packet Target Packet.
     */
    void serialize(ElemUtils::Packet &packet) const;

    /**
     * Retrieve data from given Packet.
     * @param packet Input Packet.
     */
    void unserialize(ElemUtils::Packet &packet);

    /**
     * Serialize to std::vector<double>.
     */
    void serializeIntoStdVector(std::vector<double>& vec) const;

    /**
     * Unserialize from std::vector<double>.
     */
    void unserializeFromStdVector(std::vector<double>::const_iterator& it,
            const std::vector<double>::const_iterator& end);

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

protected:

    /**
     * Default constructor.
     */
    ObservableKinematic(const std::string &className,
            ChannelType::Type channelType);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    ObservableKinematic(const ObservableKinematic &other);

    virtual void updateHashSum() const = 0;
};

/**
 * Stream operator to serialize class into Packet. See also GPDType::serialize().
 */
ElemUtils::Packet& operator <<(ElemUtils::Packet& packet,
        ObservableKinematic& observableKinematic);

/**
 * Stream operator to retrieve class from Packet. See also GPDType::unserialize().
 */
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet,
        ObservableKinematic& observableKinematic);

} /* namespace PARTONS */

#endif /* OBSERVABLE_KINEMATIC_H */
