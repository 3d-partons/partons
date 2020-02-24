#include "../../../../include/partons/utils/compare/CompareUtils.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/utils/Differences.h>
#include <NumA/utils/Tolerances.h>
#include <cmath>
#include <iostream>

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

    ComparisonData realComparisonData = compareDouble("", lhs.real(),
            rhs.real(), tolerances, "");

    ComparisonData imagComparisonData = compareDouble("", lhs.imag(),
            rhs.imag(), tolerances, "");

    return ComparisonData(
            realComparisonData.isComparisonPassed()
                    && imagComparisonData.isComparisonPassed(), variableName,
            ElemUtils::Formatter() << lhs, ElemUtils::Formatter() << rhs,
            NumA::Differences(
                    (realComparisonData.getDifferences().getAbsoluteDifference()
                            > imagComparisonData.getDifferences().getAbsoluteDifference()) ?
                            (realComparisonData.getDifferences().getAbsoluteDifference()) :
                            (imagComparisonData.getDifferences().getAbsoluteDifference()),
                    (realComparisonData.getDifferences().getRelativeDifference()
                            > imagComparisonData.getDifferences().getRelativeDifference()) ?
                            (realComparisonData.getDifferences().getRelativeDifference()) :
                            (imagComparisonData.getDifferences().getRelativeDifference())), parentObjectInfo);
}

} /* namespace PARTONS */
