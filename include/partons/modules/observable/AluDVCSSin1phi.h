#ifndef ALU_DVCS_SIN_1_PHI_H
#define ALU_DVCS_SIN_1_PHI_H

/**
 * @file AluDVCSSin1phi.h
 * @author Luca COLANERI (IPNO)
 * @date July 19, 2016
 * @version 1.0
 */

#include <string>
#include <vector>

#include "FourierObservable.h"

/**
 * @class AluDVCSSin1phi
 * 1st Fourier sin coefficient of DVCS contribution of beam spin asymmetry
 * @brief
 * described in 1210.6975v3 [hep-ph] eq. (47) + (54)
 */
class AluDVCSSin1phi: public FourierObservable {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    AluDVCSSin1phi(const std::string &className);
    virtual ~AluDVCSSin1phi();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual AluDVCSSin1phi* clone() const;

    virtual double compute();

    virtual void resolveObjectDependencies();

protected:
    /**
     * Copy constructor
     */
    AluDVCSSin1phi(const AluDVCSSin1phi &other);

    NumA::FunctionType1D* m_pFunctionToIntegrateObservable;

    virtual double functionToIntegrateObservable(double x,
            std::vector<double> params);

    void initFunctorsForIntegrations();
};

#endif /* ALU_DVCS_SIN_1_PHI_H */
