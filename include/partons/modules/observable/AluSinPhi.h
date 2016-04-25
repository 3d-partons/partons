#ifndef ALU_SIN_PHI_H
#define ALU_SIN_PHI_H

/**
 * @file AluSinPhi.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 28 September 2015
 * @version 1.0
 *
 * @class AluSinPhi
 *
 * @brief
 */

#include <string>
#include <vector>

#include "FourierObservable.h"

class Alu;

class AluSinPhi: public FourierObservable {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    AluSinPhi(const std::string &className);
    virtual ~AluSinPhi();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual AluSinPhi* clone() const;

    virtual double compute();

    virtual void resolveObjectDependencies();

protected:
    /**
     * Copy constructor
     */
    AluSinPhi(const AluSinPhi &other);

    Alu* m_pAluObservable;

    NumA::FunctionType1D* m_pFunctionToIntegrateNumObservable;
    NumA::FunctionType1D* m_pFunctionToIntegrateDenObservable;

    virtual double functionToIntegrateNumObservable(double x,
            std::vector<double> params);
    virtual double functionToIntegrateDenObservable(double x,
            std::vector<double> params);

    void initFunctorsForIntegrations();
};

#endif /* ALU_SIN_PHI_H */
