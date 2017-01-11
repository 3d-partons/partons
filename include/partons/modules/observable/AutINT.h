#ifndef AUT_INT_H
#define AUT_INT_H

/**
 * @file AcCos1phi.h
 * @author Luca COLANERI (IPNO)
 * @date July 18, 2016
 * @version 1.0
 */

#include <string>
#include <vector>

#include "../MathIntegratorModule.h"
#include "AutSinPhiPhiS.h"

/**
 * @class AutINT
 *
 *
 * 0th fourier cos coefficient of Charge Asymmetry
 *
 * described in 1210.6975v3 [hep-ph] eq. (54)
 */
class AutINT: public AutSinPhiPhiS, public MathIntegratorModule {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    AutINT(const std::string &className);
    virtual ~AutINT();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual AutINT* clone() const;

    virtual double computeFourierObservable();

protected:
    /**
     * Copy constructor
     */
    AutINT(const AutINT &other);

    NumA::FunctionType1D* m_pFunctionToIntegrateObservable;

    virtual double functionToIntegrateObservable(double x,
            std::vector<double> params);

    void initFunctorsForIntegrations();
};

#endif /* AUT_INT_H */
