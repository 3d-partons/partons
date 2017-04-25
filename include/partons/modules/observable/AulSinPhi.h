#ifndef AUL_SIN_PHI_H
#define AUL_SIN_PHI_H

/**
 * @file AulSinPhi.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date September 28, 2015
 * @version 1.0
 * @brief 1st Fourier sin coefficient of target longitudinal spin asymmetry: (\f$ N=\frac{1}{2\pi}\f$ in case n=0 ,\f$ N=\frac{1}{\pi}\f$ otherwise ): \f$ A_{UL}^{\sin(n\phi)}=N \int _{0}^{2*\pi} d\phi A_{UL}(\phi)\sin(n\phi) \f$
 */

#include <string>
#include <vector>

#include "../MathIntegratorModule.h"
#include "Aul.h"

/**
 * @class AulSinPhi
 * @brief 1st Fourier sin coefficient of target longitudinal spin asymmetry: (\f$ N=\frac{1}{2\pi}\f$ in case n=0 ,\f$ N=\frac{1}{\pi}\f$ otherwise ): \f$ A_{UL}^{\sin(n\phi)}=N \int _{0}^{2*\pi} d\phi A_{UL}(\phi)\sin(n\phi) \f$
 * described in 1210.6975v3 [hep-ph] eq. (50) + (54)
 */
class AulSinPhi: public Aul, public MathIntegratorModule {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    AulSinPhi(const std::string &className);
    virtual ~AulSinPhi();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual AulSinPhi* clone() const;

    virtual double computeFourierObservable();

protected:
    /**
     * Copy constructor
     */
    AulSinPhi(const AulSinPhi &other);

    NumA::FunctionType1D* m_pFunctionToIntegrateObservable;

    virtual double functionToIntegrateObservable(double x,
            std::vector<double> params);

    void initFunctorsForIntegrations();
};

#endif /* AUL_SIN_PHI_H */
