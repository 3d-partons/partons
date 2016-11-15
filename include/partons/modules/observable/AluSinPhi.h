#ifndef ALU_SIN_PHI_H
#define ALU_SIN_PHI_H

/**
 * @file AluSinPhi.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date September 28, 2015
 * @version 1.0
 */

#include <string>
#include <vector>

#include "../MathIntegratorModule.h"
#include "Alu.h"

/**
 * @class AluiSin1phi
 * 1st Fourier sin coefficient of beam spin asymmetry
 * @brief
 * described in 1210.6975v3 [hep-ph] eq. (48) + (54)
 */
class AluSinPhi: public Alu, public MathIntegratorModule {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    AluSinPhi(const std::string &className);
    virtual ~AluSinPhi();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual AluSinPhi* clone() const;

    virtual double computeFourierObservable();

protected:
    /**
     * Copy constructor
     */
    AluSinPhi(const AluSinPhi &other);

    NumA::FunctionType1D* m_pFunctionToIntegrateObservable;

    virtual double functionToIntegrateObservable(double x,
            std::vector<double> params);

    void initFunctorsForIntegrations();
};

#endif /* ALU_SIN_PHI_H */
