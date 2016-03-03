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

#include <complex>
#include <string>

class ComparisonData;
namespace NumA {
class Tolerances;
} /* namespace NumA */

class CompareUtils {
public:
    static ComparisonData compareDouble(const std::string &variableName,
            const double lhs, const double rhs,
            const NumA::Tolerances &tolerances,
            const std::string &parentObjectInfo);

    static ComparisonData compareComplex(const std::string &variableName,
            const std::complex<double> lhs, const std::complex<double> rhs,
            const NumA::Tolerances &tolerances,
            const std::string &parentObjectInfo);
};

#endif /* COMPARE_UTILS_H */
