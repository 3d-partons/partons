#ifndef KINEMATIC_H
#define KINEMATIC_H

/**
 * @file Kinematic.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 17, 2015
 * @version 1.0
 */

#include <string>

#include "../BaseObject.h"

namespace ElemUtils {
class Packet;
} /* namespace ElemUtils */

namespace PARTONS {

/**
 * @class Kinematic
 *
 * @brief Base class for all kinematic-like classes.
 *
 * This class is used as a base (abstract) class for all classes storing kinematics information, like e.g. GPDKinematic or ObservableKinematic.
 */
class Kinematic: public BaseObject {

public:

    /**
     * Default constructor.
     * @param className Name of class to be associated to BaseObject (see BaseObject::m_className variable)
     */
    Kinematic(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    Kinematic(const Kinematic &other);

    /**
     * Destructor.
     */
    virtual ~Kinematic();

    virtual std::string toString() const;

    // use by std::sort function
    // bool operator <(const Kinematic &other) const;

    void serialize(ElemUtils::Packet &packet) const;
    void unserialize(ElemUtils::Packet &packet);

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

//    int getListEntryPosition() const;
//    void setListEntryPosition(int listEntryPosition);

    /**
     * Get hash sum of class content.
     */
    const std::string& getHashSum() const;

    /**
     * Set hash sum of class content.
     * @param hashSum
     */
    void setHashSum(const std::string& hashSum) const;

protected:

    /**
     * Update hash sum (see Kinematic::m_hashSum variable).
     */
    virtual void updateHashSum() const = 0;

private:

    //int m_listEntryPosition;

    /**
     * Hash sum of class content.
     */
    mutable std::string m_hashSum;
};

} /* namespace PARTONS */

/**
 * Stream operator to serialize class into Packet. See also Kinematic::serialize().
 */
ElemUtils::Packet& operator <<(ElemUtils::Packet& packet, Kinematic& kinematic);

/**
 * Stream operator to retrieve class from Packet. See also Kinematic::unserialize().
 */
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet, Kinematic& kinematic);

#endif /* KINEMATIC_H */
