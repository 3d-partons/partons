#ifndef ALL_COS_1_PHI_H
#define ALL_COS_1_PHI_H

/**
 * @file AllCos1phi.h
 * @author Luca COLANERI (IPNO)
 * @date July 18, 2015
 * @version 1.0
 */

#include <string>
#include <vector>

#include "../MathIntegratorModule.h"
#include "All.h"

/**
 * @class AllCos1phi
 *
 *
 * 1st Fourier cos coefficient of double longitudinal target spin asymmetry Asymmetry
 *
 * described in 1210.6975v3 [hep-ph] eq. (51) + (54)
 */
class AllCos1phi: public All, public MathIntegratorModule {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    AllCos1phi(const std::string &className);
    virtual ~AllCos1phi();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual AllCos1phi* clone() const;

    virtual double computeFourierObservable();

protected:
    /**
     * Copy constructor
     */
    AllCos1phi(const AllCos1phi &other);

    NumA::FunctionType1D* m_pFunctionToIntegrateObservable;

    virtual double functionToIntegrateObservable(double x,
            std::vector<double> params);

    void initFunctorsForIntegrations();
};

#endif /* ALL_COS_1_PHI_H */
