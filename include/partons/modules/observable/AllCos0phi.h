#ifndef ALL_COS_0_PHI_H
#define ALL_COS_0_PHI_H

/**
 * @file AllCos0phi.h
 * @author Luca COLANERI (IPNO)
 * @date July 18, 2015
 * @version 1.0
 * @brief 0th fourier moment of target-beam spin asymmetry (\f$ N=\frac{1}{2\pi}\f$ in case n=0 ,\f$ N=\frac{1}{\pi}\f$ otherwise ): \f$ A_{LL}^{\cos(n\phi)}=N \int _{0}^{2*\pi} d\phi A_{LL}(\phi)\cos(n\phi) \f$
 *
 */

#include <string>
#include <vector>

#include "../MathIntegratorModule.h"
#include "All.h"

/**
 * @class AllCos0phi
 * @brief 0th fourier moment of target-beam spin asymmetry (\f$ N=\frac{1}{2\pi}\f$ in case n=0 ,\f$ N=\frac{1}{\pi}\f$ otherwise ): \f$ A_{LL}^{\cos(n\phi)}=N \int _{0}^{2*\pi} d\phi A_{LL}(\phi)\cos(n\phi) \f$
 *
 * 0th Fourier cos coefficient of double longitudinal target spin asymmetry
 *
 * described in 1210.6975v3 [hep-ph] eq. (51) + (54)
 */
class AllCos0phi: public All, public MathIntegratorModule {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    AllCos0phi(const std::string &className);
    virtual ~AllCos0phi();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual AllCos0phi* clone() const;

    virtual double computeFourierObservable();

protected:
    /**
     * Copy constructor
     */
    AllCos0phi(const AllCos0phi &other);

    NumA::FunctionType1D* m_pFunctionToIntegrateObservable;

    virtual double functionToIntegrateObservable(double x,
            std::vector<double> params);

    void initFunctorsForIntegrations();
};

#endif /* ALL_COS_0_PHI_H */
