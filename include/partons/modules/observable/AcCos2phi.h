#ifndef AC_COS_2_PHI_H
#define AC_COS_2_PHI_H

/**
 * @file AcCos2phi.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date September 28, 2015
 * @version 1.0
 */

#include <string>
#include <vector>

#include "../MathIntegratorModule.h"
#include "Ac.h"

namespace NumA {
class FunctionType1D;
} /* namespace NumA */

/**
 * @class AcCos2phi
 *
 *
 * 2nd Fourier cos coefficient of Charge Asymmetry
 *
 * described in 1210.6975v3 [hep-ph] eq. (54)
 */
class AcCos2phi: public Ac, public MathIntegratorModule {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    AcCos2phi(const std::string &className);
    virtual ~AcCos2phi();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual AcCos2phi* clone() const;

    virtual double computeFourierObservable();

protected:
    /**
     * Copy constructor
     */
    AcCos2phi(const AcCos2phi &other);

    NumA::FunctionType1D* m_pFunctionToIntegrateObservable;

    virtual double functionToIntegrateObservable(double x,
            std::vector<double> params);

    void initFunctorsForIntegrations();
};

#endif /* AC_COS_2_PHI_H */
