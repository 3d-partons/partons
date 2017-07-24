#include "../../../../include/partons/utils/compare/CompareUtils.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/utils/Differences.h>
#include <NumA/utils/Tolerances.h>
#include <cmath>

#include "../../../../include/partons/utils/compare/ComparisonData.h"

namespace PARTONS {


ComparisonData CompareUtils::compareDouble(const std::string &variableName,
        const double lhs, const double rhs, const NumA::Tolerances &tolerances,
        const std::string &parentObjectInfo) {

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

    return ComparisonData(comparisonResult, variableName,
            ElemUtils::Formatter() << lhs, ElemUtils::Formatter() << rhs,
            NumA::Differences(absoluteDifference, relativeDifference),
            parentObjectInfo);
}

ComparisonData CompareUtils::compareComplex(const std::string& variableName,
        const std::complex<double> lhs, const std::complex<double> rhs,
        const NumA::Tolerances& tolerances,
        const std::string& parentObjectInfo) {

    bool comparisonResult = false;

    double lhsNorm = std::norm(lhs);
    double rhsNorm = std::norm(rhs);
    double modulous = sqrt(lhsNorm);

    if (lhsNorm == rhsNorm) {
        comparisonResult = true;
    }

    //TODO replace Difference object by more useful object for complex norm/modulous
    return ComparisonData(comparisonResult, variableName,
            ElemUtils::Formatter() << lhs, ElemUtils::Formatter() << rhs,
            NumA::Differences(modulous, modulous), parentObjectInfo);
}

} /* namespace PARTONS */
