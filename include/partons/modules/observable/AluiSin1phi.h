#ifndef ALU_I_SIN_1_PHI_H
#define ALU_I_SIN_1_PHI_H

/**
 * @file AluiSin1phi.h
 * @author Luca COLANERI (IPNO)
 * @date July 19, 2016
 * @version 1.0
 */

#include <string>
#include <vector>

#include "FourierObservable.h"

/**
 * @class AluiSin1phi
 * 1st Fourier sin coefficient of Interference contribution of beam spin asymmetry
 * @brief
 * described in 1210.6975v3 [hep-ph] eq. (46) + (54)
 */
class AluiSin1phi: public FourierObservable {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    AluiSin1phi(const std::string &className);
    virtual ~AluiSin1phi();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual AluiSin1phi* clone() const;

    virtual double compute();

    virtual void resolveObjectDependencies();

protected:
    /**
     * Copy constructor
     */
    AluiSin1phi(const AluiSin1phi &other);

    NumA::FunctionType1D* m_pFunctionToIntegrateObservable;

    virtual double functionToIntegrateObservable(double x,
            std::vector<double> params);

    void initFunctorsForIntegrations();
};

#endif /* ALU_I_SIN_1_PHI_H */
