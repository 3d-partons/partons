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

#include "../../Computation.h"
#include "../../gpd/GPDType.h"
#include "DVCSConvolCoeffFunctionKinematic.h"

class DVCSConvolCoeffFunctionResult: public BaseObject, public Computation {
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
