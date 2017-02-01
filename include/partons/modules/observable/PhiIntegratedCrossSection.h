#ifndef PHI_INTEGRATED_CROSS_SECTION_H
#define PHI_INTEGRATED_CROSS_SECTION_H

/**
 * @file PhiIntegratedCrossSection.h
 * @author Luca COLANERI (IPNO)
 * @date July 18, 2015
 * @version 1.0
 */

#include <string>
#include <vector>

#include "../MathIntegratorModule.h"
#include "CrossSectionObservable.h"

/**
 * @class PhiIntegratedCrossSection
 *
 *
 * 0th Fourier cos coefficient of double longitudinal target spin asymmetry Asymmetry
 *
 * described in 1210.6975v3 [hep-ph] eq. (51) + (54)
 */
class PhiIntegratedCrossSection: public CrossSectionObservable, public MathIntegratorModule {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    PhiIntegratedCrossSection(const std::string &className);
    virtual ~PhiIntegratedCrossSection();

    /**
     * Virtual clone function to allow factory to copy all derived members
     * @return
     */
    virtual PhiIntegratedCrossSection* clone() const;

    virtual double computeFourierObservable();

protected:
    /**
     * Copy constructor
     */
    PhiIntegratedCrossSection(const PhiIntegratedCrossSection &other);

    NumA::FunctionType1D* m_pFunctionToIntegrateObservable;

    virtual double functionToIntegrateObservable(double x,
            std::vector<double> params);

    void initFunctorsForIntegrations();
};


#endif /* PHI_INTEGRATED_CROSS_SECTION_H */
