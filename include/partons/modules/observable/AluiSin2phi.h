#ifndef ALU_I_SIN_2_PHI_H
#define ALU_I_SIN_2_PHI_H

/**
 * @file AluiSin2phi.h
 * @author Luca COLANERI (IPNO)
 * @date July 19, 2016
 * @version 1.0
 */

#include <string>
#include <vector>

#include "FourierObservable.h"

/**
 * @class AluiSin2phi
 *
 * @brief
 */
class AluiSin2phi: public FourierObservable {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    AluiSin2phi(const std::string &className);
    virtual ~AluiSin2phi();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual AluiSin2phi* clone() const;

    virtual double compute();

    virtual void resolveObjectDependencies();

protected:
    /**
     * Copy constructor
     */
    AluiSin2phi(const AluiSin2phi &other);

    NumA::FunctionType1D* m_pFunctionToIntegrateObservable;

    virtual double functionToIntegrateObservable(double x,
            std::vector<double> params);

    void initFunctorsForIntegrations();
};

#endif /* ALU_I_SIN_2_PHI_H */
