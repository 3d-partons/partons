#ifndef DVCS_CONVOL_COEFF_FUNCTION_RESULT_H
#define DVCS_CONVOL_COEFF_FUNCTION_RESULT_H

/**
 * @file DVCSConvolCoeffFunctionResult.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 22 July 2015
 * @version 1.0
 *
 * @class DVCSConvolCoeffFunctionResult
 *
 * @brief
 */

#include <ElementaryUtils/string_utils/StringUtils.h>
#include <complex>
#include <map>
#include <string>
#include <vector>

#include "../../../utils/compare/ComparisonReport.h"
#include "../../gpd/GPDType.h"
#include "../../observable/ObservableChannel.h"
#include "../../Result.h"
#include "DVCSConvolCoeffFunctionKinematic.h"

class ComparisonReport;

class DVCSConvolCoeffFunctionResult: public Result {
public:
    DVCSConvolCoeffFunctionResult();
    DVCSConvolCoeffFunctionResult(DVCSConvolCoeffFunctionKinematic kinematic);
    virtual ~DVCSConvolCoeffFunctionResult();

    void add(GPDType::Type gpdComputeType, std::complex<double> result);
    std::complex<double> get(GPDType::Type gpdComputeType);

    bool isAvailable(GPDType::Type gpdType);
    std::complex<double> getLastAvailable() const;
    const std::complex<double>& getResult(GPDType::Type gpdType) const;

    void compare(ComparisonReport &rootComparisonReport,
            const DVCSConvolCoeffFunctionResult &referenceObject,
            std::string parentObjectInfo = ElemUtils::StringUtils::EMPTY) const;

    virtual std::string toString() const;

    // use by std::sort function
    bool operator <(const DVCSConvolCoeffFunctionResult &other) const;

// ##### GETTERS & SETTERS #####

    std::vector<GPDType> listGPDTypeComputed();

    const DVCSConvolCoeffFunctionKinematic& getKinematic() const;
    const std::map<GPDType::Type, std::complex<double> >& getResultsByGpdType() const;

    void setKinematic(const DVCSConvolCoeffFunctionKinematic& kinematic);
    void setResultsByGpdType(
            const std::map<GPDType::Type, std::complex<double> >& resultsByGpdType);
    const ObservableChannel& getChannel() const;
    void setChannel(const ObservableChannel& channel);

private:
    ObservableChannel m_channel;

    DVCSConvolCoeffFunctionKinematic m_kinematic;

    std::map<GPDType::Type, std::complex<double> > m_resultsByGPDType;
    std::map<GPDType::Type, std::complex<double> >::const_iterator m_it;
};

#endif /* DVCS_CONVOL_COEFF_FUNCTION_RESULT_H */
