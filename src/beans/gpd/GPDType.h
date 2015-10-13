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

class ParameterList;

class GPDType {

public:

    static const std::string GPD_TYPE_DB_COLUMN_NAME;

    enum Type {
        UNDEFINED = 0, ALL = 1, H = 2, E = 3, Ht = 4, Et = 5, HTrans = 6, ETrans = 7,
        HtTrans = 8, EtTrans = 9, H3p = 10, E3p = 11, H3pt = 12, E3pt = 13,
        H3m = 14, E3m = 15, H3mt = 16, E3mt = 17
    };

    GPDType();

    GPDType(Type type);

//    GPDType(const ParameterList &parameters);

    /**
     * Copy constructor
     * @param other
     */
    GPDType(const GPDType &other);

    operator Type() const;

    std::string toString();

    GPDType::Type getType() const;

    void setType(Type type);

    static GPDType::Type fromString(const std::string & gpdTypeStr);

private:
    GPDType::Type m_type;
};

#endif /* GPD_COMPUTE_TYPE_H */
