#ifndef KINEMATIC_TYPE_H
#define KINEMATIC_TYPE_H

/**
 * @file KinematicType.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 05 Mai 2015
 * @version 1.0
 *
 * @class KinematicType
 *
 * @brief
 */

#include <string>

class KinematicType {

public:

    enum Type {
        UNDEFINED = 0, THEO = 1, EXP = 2
    };

    KinematicType();

    KinematicType(Type type);

    /**
     * Copy constructor
     * @param other
     */
    KinematicType(const KinematicType &other);

    operator Type() const;

    std::string toString();

    KinematicType::Type getType() const;

    void setType(Type type);

private:
    KinematicType::Type m_type;
};

#endif /* KINEMATIC_TYPE_H */
