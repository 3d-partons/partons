#ifndef COMPARE_UTILS_H
#define COMPARE_UTILS_H

/**
 * @file CompareUtils.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 10 December 2015
 * @version 1.0
 *
 * @class CompareUtils
 *
 * @brief
 */

#include <string>

class ComparisonData;
class Tolerances;

class CompareUtils {
public:
    static ComparisonData compareDouble(const std::string &variableName,
            const double lhs, const double rhs, const Tolerances &tolerances);
};

#endif /* COMPARE_UTILS_H */
