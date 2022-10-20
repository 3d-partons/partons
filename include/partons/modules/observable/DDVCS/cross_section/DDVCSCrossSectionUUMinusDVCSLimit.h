#ifndef DDVCSCROSSSECTIONUUMINUSDVCSLIMIT_H
#define DDVCSCROSSSECTIONUUMINUSDVCSLIMIT_H

/**
 * @file DDVCSCrossSectionUUMinusDVCSLimit.h
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
 * @class DDVCSCrossSectionUUMinusDVCSLimit
 * @brief Unpolarized cross-section for negative beam charge - DVCS limit.
 *
 * Unit: \f$\mathrm{nbarn}/\mathrm{GeV}^4\f$.
 */
class DDVCSCrossSectionUUMinusDVCSLimit: public DDVCSObservable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Function for the integration.
     */
    static double DDVCSCrossSectionUUMinusDVCSLimitFunction(double* kin,
            size_t dim, void* par);

    /**
     * Constructor.
     * @param className Name of class.
     */
    DDVCSCrossSectionUUMinusDVCSLimit(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DDVCSCrossSectionUUMinusDVCSLimit();

    virtual DDVCSCrossSectionUUMinusDVCSLimit* clone() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DDVCSCrossSectionUUMinusDVCSLimit(
            const DDVCSCrossSectionUUMinusDVCSLimit &other);

    virtual PhysicalType<double> computeObservable(
            const DDVCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType);
};

struct DDVCSCrossSectionUUMinusDVCSLimitParameters {

    DDVCSCrossSectionUUMinusDVCSLimit* m_pDDVCSCrossSectionUUMinusDVCSLimit; ///< Pointer to DDVCSCrossSectionUUMinusDVCSLimit.
    double m_xB, m_t, m_Q2, m_Q2Prim, m_E, m_phi;
    List<GPDType> m_gpdType; ///< GPD types.
};

} /* namespace PARTONS */

#endif /* DDVCSCROSSSECTIONUUMINUSDVCSLIMIT_H */
