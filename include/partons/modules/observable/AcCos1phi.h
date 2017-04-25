#ifndef AC_COS_1_PHI_H
#define AC_COS_1_PHI_H

/**
 * @class AcCos1phi
 * @author Luca COLANERI (IPNO)
 * @date July 18, 2016
 * @version 1.0
 * @brief 1st fourier moment of charge asymmetry (\f$ N=\frac{1}{2\pi}\f$ in case n=0 ,\f$ N=\frac{1}{\pi}\f$ otherwise ): \f$ A_{C}^{\cos(n\phi)}=N \int _{0}^{2*\pi} d\phi A_{C}(\phi)\cos(n\phi) \f$
 */

#include <string>
#include <vector>

#include "../MathIntegratorModule.h"
#include "Ac.h"

/**
 * @class AcCos1phi
 *
 *
 * 1st Fourier cos coefficient of Charge Asymmetry
 *
 * described in 1210.6975v3 [hep-ph] eq. (54)
 */
class AcCos1phi: public Ac, public MathIntegratorModule {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    AcCos1phi(const std::string &className);
    virtual ~AcCos1phi();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual AcCos1phi* clone() const;

    virtual double computeFourierObservable();

protected:
    /**
     * Copy constructor
     */
    AcCos1phi(const AcCos1phi &other);

    NumA::FunctionType1D* m_pFunctionToIntegrateObservable;

    virtual double functionToIntegrateObservable(double x,
            std::vector<double> params);

    void initFunctorsForIntegrations();
};

#endif /* AC_COS_1_PHI_H */
