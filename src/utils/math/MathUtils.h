/*
 * @file MathUtils.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 02 December 2014
 * @version 1.0
 *
 * @class MathUtils
 */

#ifndef MATH_UTILS_H
#define MATH_UTILS_H

class Tolerances;
class DoubleComparisonReport;

# define M_PI           3.14159265358979323846  /* pi */

class MathUtils {
public:
    static double convertDegreeToRadian(double degree);

    static void isPositiveDouble(double tolerance);

    static DoubleComparisonReport compare(double lhs, double rhs,
            double absoluteTolerance, double relativeTolerance,
            bool testTolerances = true);

    static DoubleComparisonReport compare(double lhs, double rhs,
            const Tolerances &tolerances);
};

#endif /* MATH_UTILS_H */
