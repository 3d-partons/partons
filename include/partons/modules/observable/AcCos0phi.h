#ifndef AC_COS_0_PHI_H
#define AC_COS_0_PHI_H

/**
 * @file AcCos1phi.h
 * @author Luca COLANERI (IPNO)
 * @date July 18, 2016
 * @version 1.0
 */

#include <string>
#include <vector>

#include "../MathIntegratorModule.h"
#include "Ac.h"

/**
 * @class AcCos0phi
 *
 *
 * 0th fourier cos coefficient of Charge Asymmetry
 *
 * described in 1210.6975v3 [hep-ph] eq. (54)
 */
class AcCos0phi: public Ac, public MathIntegratorModule {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    AcCos0phi(const std::string &className);
    virtual ~AcCos0phi();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual AcCos0phi* clone() const;

    virtual double computeFourierObservable();

protected:
    /**
     * Copy constructor
     */
    AcCos0phi(const AcCos0phi &other);

    NumA::FunctionType1D* m_pFunctionToIntegrateObservable;

    virtual double functionToIntegrateObservable(double x,
            std::vector<double> params);

    void initFunctorsForIntegrations();
};

#endif /* AC_COS_0_PHI_H */
