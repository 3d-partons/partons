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

#include <complex>
#include <map>
#include <string>
#include <vector>

#include "../../gpd/GPDType.h"
#include "../../Result.h"
#include "DVCSConvolCoeffFunctionKinematic.h"

namespace NumA {
class Tolerances;
} /* namespace NumA */

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
            const NumA::Tolerances &tolerances,
            std::string parentObjectInfo) const;

    std::string toString();

// ##### GETTERS & SETTERS #####

    std::vector<GPDType> listGPDTypeComputed();

    const DVCSConvolCoeffFunctionKinematic& getKinematic() const;
    const std::map<GPDType::Type, std::complex<double> >& getResultsByGpdType() const;

    void setKinematic(const DVCSConvolCoeffFunctionKinematic& kinematic);
    void setResultsByGpdType(
            const std::map<GPDType::Type, std::complex<double> >& resultsByGpdType);

private:
    DVCSConvolCoeffFunctionKinematic m_kinematic;

    std::map<GPDType::Type, std::complex<double> > m_resultsByGPDType;
    std::map<GPDType::Type, std::complex<double> >::const_iterator m_it;
};

#endif /* DVCS_CONVOL_COEFF_FUNCTION_RESULT_H */
