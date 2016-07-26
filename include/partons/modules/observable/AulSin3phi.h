#ifndef AUL_SIN_3_PHI_H
#define AUL_SIN_3_PHI_H

/**
 * @file AulSin3phi.h
 * @author Luca COLANERI (IPNO)
 * @date July 18, 2016
 * @version 1.0
 */

#include <string>
#include <vector>

#include "FourierObservable.h"

/**
 * @class AulSin3phi
 * 3rd Fourier sin coefficient of target longitudinal spin asymmetry
 * @brief
 * described in 1210.6975v3 [hep-ph] eq. (50) + (54)
 */
class AulSin3phi: public FourierObservable {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    AulSin3phi(const std::string &className);
    virtual ~AulSin3phi();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual AulSin3phi* clone() const;

    virtual double compute();

    virtual void resolveObjectDependencies();

protected:
    /**
     * Copy constructor
     */
    AulSin3phi(const AulSin3phi &other);

    NumA::FunctionType1D* m_pFunctionToIntegrateObservable;

    virtual double functionToIntegrateObservable(double x,
            std::vector<double> params);

    void initFunctorsForIntegrations();
};

#endif /* AUL_SIN_3_PHI_H */
