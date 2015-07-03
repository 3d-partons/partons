#include "MathUtils.h"

#include <cmath>
#include <sstream>
#include <stdexcept>
#include <string>

#include "../test/DoubleComparisonReport.h"

double MathUtils::convertDegreeToRadian(double degree) {
    return degree * (M_PI / 180.);
}

DoubleComparisonReport MathUtils::compare(double lhs, double rhs,
        double absoluteTolerance, double relativeTolerance) {

    // Check that tolerances are positive
    std::ostringstream absoluteToleranceStream;
    absoluteToleranceStream << absoluteTolerance;
    std::string absoluteToleranceString = absoluteToleranceStream.str();
    std::ostringstream relativeToleranceStream;
    relativeToleranceStream << relativeTolerance;
    std::string relativeToleranceString = relativeToleranceStream.str();
    std::string ErrorMessage =
            "Tolerances should be >0. Here Absolute Tolerance = "
                    + absoluteToleranceString + " and Relative Tolerance = "
                    + relativeToleranceString;

    if (absoluteTolerance <= 0. || relativeTolerance <= 0.)
        throw std::range_error(ErrorMessage);

    // Define and evaluate relative and absolute differences
    double absoluteDifference = fabs(lhs - rhs);
    double relativeDifference;

    if (lhs != 0) {
        relativeDifference = absoluteDifference / fabs(lhs);
    }
    if (lhs == 0 && rhs != 0) {
        relativeDifference = absoluteDifference / fabs(rhs);
    }
    if (lhs == 0 && rhs == 0) {
        relativeDifference = 0.;
    }

    // Test differences within tolerances
    bool comparisonResult = false;

    if (absoluteDifference < absoluteTolerance
            && relativeDifference < relativeTolerance)
        comparisonResult = true;

    return DoubleComparisonReport(comparisonResult, absoluteDifference,
            relativeDifference, absoluteTolerance, relativeTolerance);
}
