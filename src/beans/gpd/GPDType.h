#ifndef GPD_TYPE_H
#define GPD_TYPE_H

/**
 * @file GPDType.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 30 July 2014
 * @version 1.0
 *
 * @class GPDType
 *
 * @brief
 */

#include <string>

class GPDType {

public:

    static const std::string GPD_TYPE_DB_COLUMN_NAME;

    enum Type {
        UNDEFINED, ALL, H, Ht, E, Et, HTrans
    };

    GPDType();

    GPDType(Type type);

    /**
     * Copy constructor
     * @param other
     */
    GPDType(const GPDType &other);

    operator Type() const;

    std::string toString();

    GPDType::Type getType() const;

    void setType(Type type);

private:
    GPDType::Type m_type;
};

#endif /* GPD_COMPUTE_TYPE_H */
