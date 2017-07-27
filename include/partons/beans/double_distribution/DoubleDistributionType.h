#ifndef DOUBLE_DISTRIBUTION_TYPE_H
#define DOUBLE_DISTRIBUTION_TYPE_H

/**
 * @file DoubleDistributionType.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 29 July 2015
 * @version 1.0
 */

#include <string>

namespace PARTONS {

/**
 * @class DoubleDistributionType
 *
 * @brief
 */
class DoubleDistributionType {
public:
    enum Type {
        UNDEFINED, ALL, F, G, K, fM, fP
    };

    DoubleDistributionType();
    DoubleDistributionType(DoubleDistributionType::Type type);
    virtual ~DoubleDistributionType();

    operator Type() const;

    std::string toString() const;

private:
    DoubleDistributionType::Type m_type;
};

} /* namespace PARTONS */

#endif /* DOUBLE_DISTRIBUTION_TYPE_H */
