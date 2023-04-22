#ifndef DDVCSCROSSSECTIONUUMINUSTCSLIMIT_H
#define DDVCSCROSSSECTIONUUMINUSTCSLIMIT_H

/**
 * @file DDVCSCrossSectionUUMinusTCSLimit.h
 * @author Pawel Sznajder (IPNO)
 * @date November 25, 2016
 * @version 1.0
 */

#include <stddef.h>
#include <string>

#include "../../../../beans/gpd/GPDType.h"
#include "../../../../beans/List.h"
#include "../../../../utils/type/PhysicalType.h"
#include "../DDVCSObservable.h"

namespace PARTONS {

/**
 * @class DDVCSCrossSectionUUMinusTCSLimit
 * @brief Unpolarized cross-section for negative beam charge - DVCS limit.
 *
 * Unit: \f$\mathrm{nbarn}/\mathrm{GeV}^4\f$.
 */
class DDVCSCrossSectionUUMinusTCSLimit: public DDVCSObservable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Function for the integration.
     */
    static double DDVCSCrossSectionUUMinusTCSLimitFunction(double* kin,
            size_t dim, void* par);

    /**
     * Constructor.
     * @param className Name of class.
     */
    DDVCSCrossSectionUUMinusTCSLimit(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DDVCSCrossSectionUUMinusTCSLimit();

    virtual DDVCSCrossSectionUUMinusTCSLimit* clone() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DDVCSCrossSectionUUMinusTCSLimit(
            const DDVCSCrossSectionUUMinusTCSLimit &other);

    virtual PhysicalType<double> computeObservable(
            const DDVCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType);
};

struct DDVCSCrossSectionUUMinusTCSLimitParameters {

    DDVCSCrossSectionUUMinusTCSLimit* m_pDDVCSCrossSectionUUMinusTCSLimit; ///< Pointer to DDVCSCrossSectionUUMinusTCSLimit.
    double m_xB, m_t, m_Q2, m_Q2Prim, m_E, m_phiL, m_thetaL;
    List<GPDType> m_gpdType; ///< GPD types.
};

} /* namespace PARTONS */

#endif /* DDVCSCROSSSECTIONUUMINUSTCSLIMIT_H */
