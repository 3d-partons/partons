#ifndef PHYSICAL_UNIT_H
#define PHYSICAL_UNIT_H

/**
 * @file PhysicalUnit.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 20 January 2016
 * @version 1.0
 *
 * @class PhysicalUnit
 *
 * @brief
 */

#include <string>

class PhysicalUnit {
public:
    enum Type {
        NONE = 0, GEV = 1, GEV2 = 2, DEGREE = 3
    };

    PhysicalUnit();

    PhysicalUnit(Type type);

    operator Type() const;

    std::string toString() const;

    std::string getShortName();

    PhysicalUnit::Type getType() const;

    void setType(Type type);

private:
    PhysicalUnit::Type m_type;
};

#endif /* PHYSICAL_UNIT_H */
