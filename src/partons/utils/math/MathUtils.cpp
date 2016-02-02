#include "../../../../include/partons/utils/math/MathUtils.h"

#include <math.h>
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

//TODO DiLog function come from ROOT CERN library ; check licence
double MathUtils::DiLog(double x) {
    // The DiLogarithm function
    // Code translated by R.Brun from CERNLIB DILOG function C332

    const double hf = 0.5;
    const double pi2 = M_PI * M_PI;
    const double pi3 = pi2 / 3;
    const double pi6 = pi2 / 6;
    const double pi12 = pi2 / 12;
    const double c[20] = { 0.42996693560813697, 0.40975987533077105,
            -0.01858843665014592, 0.00145751084062268, -0.00014304184442340,
            0.00001588415541880, -0.00000190784959387, 0.00000024195180854,
            -0.00000003193341274, 0.00000000434545063, -0.00000000060578480,
            0.00000000008612098, -0.00000000001244332, 0.00000000000182256,
            -0.00000000000027007, 0.00000000000004042, -0.00000000000000610,
            0.00000000000000093, -0.00000000000000014, 0.00000000000000002 };

    double t, h, y, s, a, alfa, b1, b2, b0;

    if (x == 1) {
        h = pi6;
    } else if (x == -1) {
        h = -pi12;
    } else {
        t = -x;
        if (t <= -2) {
            y = -1 / (1 + t);
            s = 1;
            b1 = log(-t);
            b2 = log(1 + 1 / t);
            a = -pi3 + hf * (b1 * b1 - b2 * b2);
        } else if (t < -1) {
            y = -1 - t;
            s = -1;
            a = log(-t);
            a = -pi6 + a * (a + log(1 + 1 / t));
        } else if (t <= -0.5) {
            y = -(1 + t) / t;
            s = 1;
            a = log(-t);
            a = -pi6 + a * (-hf * a + log(1 + t));
        } else if (t < 0) {
            y = -t / (1 + t);
            s = -1;
            b1 = log(1 + t);
            a = hf * b1 * b1;
        } else if (t <= 1) {
            y = t;
            s = 1;
            a = 0;
        } else {
            y = 1 / t;
            s = -1;
            b1 = log(t);
            a = pi6 + hf * b1 * b1;
        }
        h = y + y - 1;
        alfa = h + h;
        b1 = 0;
        b2 = 0;
        for (short i = 19; i >= 0; i--) {
            b0 = c[i] + alfa * b1 - b2;
            b2 = b1;
            b1 = b0;
        }
        h = -(s * (b0 - h * b2) + a);
    }
    return h;
}
