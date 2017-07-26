#ifndef COMPARE_UTILS_H
#define COMPARE_UTILS_H

/**
 * @file CompareUtils.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 10 December 2015
 * @version 1.0
 */

#include <complex>
#include <string>

namespace NumA {
class Tolerances;
} /* namespace NumA */

namespace PARTONS {

class ComparisonData;

/**
* @class CompareUtils
*
* @brief Set of utility tools to perform comparisons.
*
* This class defines a set of useful tools to perform systematic comparisons between module results.
*/
class CompareUtils {

public:

	/**
	 * Compare two double precision numbers.
	 * @param variableName Name of variable.
	 * @param lhs Left-hand-size value.
	 * @param rhs Right-hand-side value.
	 * @param tolerances Tolerance assumed in the comparison.
	 * @param parentObjectInfo Information on object being compared.
	 * @return Result of the comparison encapsulated in ComparisonData object.
	 */
    static ComparisonData compareDouble(const std::string &variableName,
            const double lhs, const double rhs,
            const NumA::Tolerances &tolerances,
            const std::string &parentObjectInfo);

	/**
	 * Compare two complex double precision numbers.
	 * @param variableName Name of variable.
	 * @param lhs Left-hand-size value.
	 * @param rhs Right-hand-side value.
	 * @param tolerances Tolerance assumed in the comparison.
	 * @param parentObjectInfo Information on object being compared.
	 * @return Result of the comparison encapsulated in ComparisonData object.
	 */
    static ComparisonData compareComplex(const std::string &variableName,
            const std::complex<double> lhs, const std::complex<double> rhs,
            const NumA::Tolerances &tolerances,
            const std::string &parentObjectInfo);
};

} /* namespace PARTONS */

#endif /* COMPARE_UTILS_H */
