#ifndef DOUBLE_H
#define DOUBLE_H

/**
 * @file Double.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 19 January 2016
 * @version 1.0
 */

#include "BaseType.h"

namespace PARTONS {

/**
 * @class Double
 *
 * @brief
 */
class Double: public BaseType {
public:
    Double();
    Double(double value);
    virtual ~Double();

    /// Arithmetic operators

private:
    double m_value;
};

} /* namespace PARTONS */

#endif /* DOUBLE_H */
