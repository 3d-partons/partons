#ifndef KINEMATIC_H
#define KINEMATIC_H

/**
 * @file Kinematic.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 17, 2015
 * @version 1.0
 */

#include <string>

#include "../database/DatabaseObject.h"

namespace ElemUtils {
class Packet;
} /* namespace ElemUtils */

/**
 * @class Kinematic
 *
 * @brief
 */
class Kinematic: public DatabaseObject {
public:
    Kinematic(const std::string &className);
    virtual ~Kinematic();

    virtual std::string toString() const;

    // use by std::sort function
    bool operator <(const Kinematic &other) const;

    void serialize(ElemUtils::Packet &packet) const;
    void unserialize(ElemUtils::Packet &packet);

    int getListEntryPosition() const;
    void setListEntryPosition(int listEntryPosition);

    const std::string& getHashSum() const;
    void setHashSum(const std::string& hashSum) const;

protected:
    virtual void updateHashSum() const = 0;

private:
    int m_listEntryPosition;
    mutable std::string m_hashSum;
};

ElemUtils::Packet& operator <<(ElemUtils::Packet& packet, Kinematic& kinematic);
ElemUtils::Packet& operator >>(ElemUtils::Packet& packet, Kinematic& kinematic);

#endif /* KINEMATIC_H */
