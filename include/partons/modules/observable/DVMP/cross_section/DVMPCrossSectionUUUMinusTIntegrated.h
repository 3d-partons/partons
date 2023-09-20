#ifndef DVMPCROSSSECTIONUUUMINUSTINTEGRATED_H
#define DVMPCROSSSECTIONUUUMINUSTINTEGRATED_H

/**
 * @file DVMPCrossSectionUUUMinusTIntegrated.h
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
#include "DVMPCrossSectionUUUMinus.h"

namespace PARTONS {

/**
 * @class DVMPCrossSectionUUUMinusTIntegrated
 * @brief Unpolarized cross-section for electro-production integrated over \f$\phi\f$.
 *
 * Unit: \f$\mathrm{nbarn}/\mathrm{GeV}^4\f$.
 */
class DVMPCrossSectionUUUMinusTIntegrated: public DVMPCrossSectionUUUMinus,
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
    DVMPCrossSectionUUUMinusTIntegrated(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVMPCrossSectionUUUMinusTIntegrated();

    virtual DVMPCrossSectionUUUMinusTIntegrated* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVMPCrossSectionUUUMinusTIntegrated(
            const DVMPCrossSectionUUUMinusTIntegrated &other);

    virtual PhysicalType<double> computeObservable(
            const DVMPObservableKinematic& kinematic,
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

private:

    double m_tLimit; ///< Minimum value of t used in integration.
};

} /* namespace PARTONS */

#endif /* DVMPCROSSSECTIONUUUMINUSTINTEGRATED_H */
