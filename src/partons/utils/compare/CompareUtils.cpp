#include "../../../../include/partons/utils/compare/CompareUtils.h"

#include <cmath>

#include "../../../../include/partons/utils/compare/ComparisonData.h"
#include "../../../../include/partons/utils/compare/Differences.h"
#include "../../../../include/partons/utils/math/Tolerances.h"
#include "../../../../include/partons/utils/stringUtils/Formatter.h"

ComparisonData CompareUtils::compareDouble(const std::string &variableName,
        const double lhs, const double rhs, const Tolerances &tolerances) {

    bool comparisonResult = false;

    // Define and evaluate relative and absolute differences
    double absoluteDifference = std::fabs(lhs - rhs);
    double relativeDifference = 0.;

    if (lhs != 0) {
        relativeDifference = absoluteDifference / std::fabs(lhs);
    }
    if (lhs == 0 && rhs != 0) {
        relativeDifference = absoluteDifference / std::fabs(rhs);
    }

    // Test differences within tolerances
    if (absoluteDifference < tolerances.getAbsoluteTolerance()
            && relativeDifference < tolerances.getRelativeTolerance()) {
        comparisonResult = true;
    }

    return ComparisonData(comparisonResult, variableName, Formatter() << lhs,
            Formatter() << rhs, new Tolerances(tolerances),
            new Differences(absoluteDifference, relativeDifference));
}
