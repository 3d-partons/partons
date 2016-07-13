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

#include "FourierObservable.h"

/**
 * @class AluSinPhi
 *
 * @brief
 */
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

    Observable* m_pAluObservable;

    NumA::FunctionType1D* m_pFunctionToIntegrateObservable;

    virtual double functionToIntegrateObservable(double x,
            std::vector<double> params);

    void initFunctorsForIntegrations();
};

#endif /* ALU_SIN_PHI_H */
