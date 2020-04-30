#ifndef DVCSCROSSSECTIONUUMINUSPHIINTEGRATED_H
#define DVCSCROSSSECTIONUUMINUSPHIINTEGRATED_H

/**
 * @file DVCSCrossSectionUUMinusPhiIntegrated.h
 * @author Luca COLANERI (IPNO)
 * @date April 24, 2017
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>

#include "../../../../beans/gpd/GPDType.h"
#include "../../../../beans/List.h"
#include "../../../../utils/type/PhysicalType.h"
#include "../../../MathIntegratorModule.h"
#include "DVCSCrossSectionUUMinus.h"

namespace PARTONS {

/**
 * @class DVCSCrossSectionUUMinusPhiIntegrated
 * @brief Unpolarized cross-section for electro-production integrated over \f$\phi\f$.
 *
 * Unit: \f$\mathrm{nbarn}/\mathrm{GeV}^4\f$.
 */
class DVCSCrossSectionUUMinusPhiIntegrated: public DVCSCrossSectionUUMinus,
        public MathIntegratorModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVCSCrossSectionUUMinusPhiIntegrated(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVCSCrossSectionUUMinusPhiIntegrated();

    virtual DVCSCrossSectionUUMinusPhiIntegrated* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVCSCrossSectionUUMinusPhiIntegrated(
            const DVCSCrossSectionUUMinusPhiIntegrated &other);

    virtual PhysicalType<double> computeObservable(
            const DVCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType);

    /**
     * Functor to perform the integration.
     */
    NumA::FunctionType1D* m_pFunctionToIntegrateObservable;

    /**
     * Function to be integrated.
     */
    virtual double functionToIntegrateObservable(double x,
            std::vector<double> params);

    /**
     * Initialize functors.
     */
    void initFunctorsForIntegrations();
};

} /* namespace PARTONS */

#endif /* DVCSCROSSSECTIONUUMINUSPHIINTEGRATED_H */
