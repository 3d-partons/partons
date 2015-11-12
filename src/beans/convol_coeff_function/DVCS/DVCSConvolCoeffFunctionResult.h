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

#include "../../gpd/GPDType.h"
#include "../../Result.h"
#include "DVCSConvolCoeffFunctionKinematic.h"

//TODO improve public BaseObject, public Result
class DVCSConvolCoeffFunctionResult: public BaseObject, public Result {
public:
    DVCSConvolCoeffFunctionResult();
    DVCSConvolCoeffFunctionResult(DVCSConvolCoeffFunctionKinematic kinematic);
    virtual ~DVCSConvolCoeffFunctionResult();

    void add(GPDType::Type gpdComputeType, std::complex<double> result);
    std::complex<double> get(GPDType::Type gpdComputeType);

    bool isAvailable(GPDType gpdType);
    std::complex<double> getLastAvailable();

    std::string toString();

// ##### GETTERS & SETTERS #####

    const DVCSConvolCoeffFunctionKinematic& getKinematic() const;
    void setKinematic(const DVCSConvolCoeffFunctionKinematic& kinematic);
    const std::map<GPDType::Type, std::complex<double> >& getResultsByGpdType() const;
    void setResultsByGpdType(
            const std::map<GPDType::Type, std::complex<double> >& resultsByGpdType);

private:
    DVCSConvolCoeffFunctionKinematic m_kinematic;

    std::map<GPDType::Type, std::complex<double> > m_resultsByGPDType;
    std::map<GPDType::Type, std::complex<double> >::const_iterator m_it;
};

#endif /* DVCS_CONVOL_COEFF_FUNCTION_RESULT_H */
