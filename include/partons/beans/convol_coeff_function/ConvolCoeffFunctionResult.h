#ifndef CONVOL_COEFF_FUNCTION_RESULT_H
#define CONVOL_COEFF_FUNCTION_RESULT_H

/**
 * @file ConvolCoeffFunctionResult.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 22 July 2015
 * @version 1.0
 */

#include <ElementaryUtils/string_utils/StringUtils.h>
#include <complex>
#include <map>
#include <string>
#include <vector>

#include "../../utils/compare/ComparisonReport.h"
#include "../channel/ChannelType.h"
#include "../gpd/GPDType.h"
#include "../Result.h"

namespace PARTONS {

/**
 * @class ConvolCoeffFunctionResult
 *
 * @brief Abstract class representing a single result of CCF computation.
 *
 * This abstract class is used to store results of a single CCF computation.
 */
class ConvolCoeffFunctionResult: public Result {

public:

    /**
     * Default constructor.
     */
    ConvolCoeffFunctionResult(const std::string &className,
            ChannelType::Type channelType);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    ConvolCoeffFunctionResult(const ConvolCoeffFunctionResult& other);

    /**
     * Destructor.
     */
    virtual ~ConvolCoeffFunctionResult();

    virtual std::string toString() const;

    /**
     * Add result for given GPD type.
     * @param gpdComputeType GPD type associated to given result.
     * @param result Result to be added.
     */
    void addResult(GPDType::Type gpdType, const std::complex<double>& result);

    /**
     * Get reference to result for given GPD type.
     * @param gpdType GPD type associated to requested result.
     * @return Reference to requested result.
     */
    const std::complex<double>& getResult(GPDType::Type gpdType) const;

    /**
     * Check if the object stores result associated to given GPD type.
     * @param gpdType GPD type to be checked.
     * @return True if the object stores result associated to given GPD type, otherwise false.
     * @see ConvolCoeffFunctionResult::getLastAvailable()
     */
    bool isAvailable(GPDType::Type gpdType);

    /**
     * Get reference to result marked by the last call of ConvolCoeffFunctionResult::isAvailible() function.
     * @return Reference to selected result.
     * @see ConvolCoeffFunctionResult::isAvailible()
     */
    std::complex<double> getLastAvailable() const;

    /**
     * Compare to other ConvolCoeffFunctionResult object and store comparison result in given comparison report.
     * @param rootComparisonReport Reference to comparison report to be used to store comparison result.
     * @param referenceObject Reference to object to be compared.
     * @param parentObjectInfo Addition information coming from the parent object (if needed).
     */
    void compare(ComparisonReport &rootComparisonReport,
            const ConvolCoeffFunctionResult &referenceObject,
            std::string parentObjectInfo = ElemUtils::StringUtils::EMPTY) const;

    /**
     * Get list of GPD types associated to stored parton distributions.
     * @return Vector of associated types.
     */
    std::vector<GPDType> listGPDTypeComputed();

    //********************************************************
    //*** SETTERS AND GETTERS ********************************
    //********************************************************

    /**
     * Get reference to map containing stored results distinguished by associated GPD types.
     */
    const std::map<GPDType::Type, std::complex<double> >& getResultsByGpdType() const;

    /**
     * Set map containing stored results distinguished by associated GPD types.
     */
    void setResultsByGpdType(
            const std::map<GPDType::Type, std::complex<double> >& resultsByGpdType);

private:

    /**
     * Map containing stored results distinguished by associated GPD types.
     */
    std::map<GPDType::Type, std::complex<double> > m_resultsByGPDType;

    /**
     * Iterator used to mark a specific entry in ConvolCoeffFunctionResult::m_resultsByGPDType.
     */
    std::map<GPDType::Type, std::complex<double> >::const_iterator m_it;
};

} /* namespace PARTONS */

#endif /* CONVOL_COEFF_FUNCTION_RESULT_H */
