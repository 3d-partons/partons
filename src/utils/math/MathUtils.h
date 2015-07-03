#ifndef MATH_UTILS_H
#define MATH_UTILS_H

/**
 * @file MathUtils.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 02 December 2014
 * @version 1.0
 *
 * @class MathUtils
 */
class DoubleComparisonReport;

# define M_PI           3.14159265358979323846  /* pi */

class MathUtils {
public:
    static double convertDegreeToRadian(double degree);

    // TODO: Introduce object for tolerances to avoid duplication of tests of positive tolerances
    static DoubleComparisonReport compare(double lhs, double rhs,
            double absoluteTolerance, double relativeTolerance);
};

#endif /* MATH_UTILS_H */
