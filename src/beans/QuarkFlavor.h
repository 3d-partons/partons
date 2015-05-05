#ifndef QUARK_FLAVOR_H
#define QUARK_FLAVOR_H

/**
 *
 * @file QuarkFlavor.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 22 July 2014
 * @version 1.0
 *
 * @class QuarkFlavor
 *
 * @brief Defines existing quark flavors and associates them with an enum type.\n
 * It represents the dependence on quark flavors of hadron structure distributions.
 * Current values are:\n
 * - UNDEFINED: empty field\n
 * - UP, DOWN, STRANGE, CHARM, BOTTOM, TOP: explicit quark names\n
 */

#include <string>

class QuarkFlavor {
public:

    static const std::string QUARK_FLAVOR_TYPE_DB_COLUMN_NAME;

    enum Type {
        UNDEFINED, UP, DOWN, STRANGE, CHARM, BOTTOM, TOP
    };

    QuarkFlavor();

    QuarkFlavor(Type type);

    operator Type() const;

    std::string toString();

    std::string getShortName();

    QuarkFlavor::Type getType() const;

    void setType(Type type);

private:
    QuarkFlavor::Type m_type;
};

#endif /* QUARK_FLAVOR_H */
