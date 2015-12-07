#include "../../../../include/partons/utils/math/MathUtils.h"

#include <cmath>
#include <sstream>
#include <stdexcept>
#include <string>

#include "../../../../include/partons/utils/math/Tolerances.h"
#include "../../../../include/partons/utils/test/DoubleComparisonReport.h"

double MathUtils::convertDegreeToRadian(double degree) {
    return degree * (M_PI / 180.);
}

void MathUtils::isPositiveDouble(double tolerance) {

    // Check that tolerance is positive
    std::ostringstream toleranceStream;
    toleranceStream << tolerance;
    std::string toleranceString = toleranceStream.str();
    std::string ErrorMessage = "Tolerance should be >0. Here tolerance = "
            + toleranceString;

    if (tolerance <= 0.)
        throw std::range_error(ErrorMessage);
}

DoubleComparisonReport MathUtils::compare(double lhs, double rhs,
        double absoluteTolerance, double relativeTolerance,
        bool testTolerances) {

    bool comparisonResult = false;

    // Define and evaluate relative and absolute differences
    double absoluteDifference = fabs(lhs - rhs);
    double relativeDifference = 0.;

    if (lhs != 0) {
        relativeDifference = absoluteDifference / fabs(lhs);
    }
    if (lhs == 0 && rhs != 0) {
        relativeDifference = absoluteDifference / fabs(rhs);
    }

    // Check that tolerances are positive
    if (testTolerances) {
        isPositiveDouble(absoluteTolerance);
        isPositiveDouble(relativeTolerance);
    }

    // Test differences within tolerances
    if (absoluteDifference < absoluteTolerance
            && relativeDifference < relativeTolerance)
        comparisonResult = true;

    return DoubleComparisonReport(comparisonResult, absoluteDifference,
            relativeDifference, absoluteTolerance, relativeTolerance);
}

DoubleComparisonReport MathUtils::compare(double lhs, double rhs,
        const Tolerances &tolerances) {

    return compare(lhs, rhs, tolerances.getAbsoluteTolerance(),
            tolerances.getRelativeTolerance(), false);
}
