#ifndef TCSR_H
#define TCSR_H

/**
 * @file TCSR.h
 * @author Pawel Sznajder (IPNO)
 * @date November 25, 2016
 * @version 1.0
 */

#include <string>
#include <vector>

#include "../../../../beans/gpd/GPDType.h"
#include "../../../../beans/List.h"
#include "../../../../utils/type/PhysicalType.h"
#include "../cross_section/TCSCrossSectionUUWeightedThetaIntegrated.h"

namespace PARTONS {

/**
 * @class TCSR
 * @brief Asymmetry R.
 *
 * Definition:<br>
 *
 * see Eq. (31) of Eur. Phys. J. C23 (2002) 675
 *
 * Unit: none.
 */
class TCSR: public TCSCrossSectionUUWeightedThetaIntegrated {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    TCSR(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~TCSR();

    virtual TCSR* clone() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    TCSR(const TCSR &other);

    virtual PhysicalType<double> computeObservable(
            const TCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType);

    /**
     * Functor to perform the integration (numerator).
     */
    NumA::FunctionType1D* m_pFunctionToIntegrateObservablePhi1;

    /**
     * Functor to perform the integration (denominator).
     */
    NumA::FunctionType1D* m_pFunctionToIntegrateObservablePhi2;

    /**
     * Function to be integrated (numerator).
     */
    virtual double functionToIntegrateObservablePhi1(double x,
            std::vector<double> params);

    /**
     * Function to be integrated (denominator).
     */
    virtual double functionToIntegrateObservablePhi2(double x,
            std::vector<double> params);

    /**
     * Initialize functors.
     */
    virtual void initFunctorsForIntegrations();
};

} /* namespace PARTONS */

#endif /* TCSR_H */
