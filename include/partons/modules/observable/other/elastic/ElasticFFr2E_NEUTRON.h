#ifndef DVCSCFFXIREH_H
#define DVCSCFFXIREH_H

/**
 * @file DVCSCFFReH.h
 * @author Pawel Sznajder (NCBJ)
 * @date December 15, 2025
 * @version 1.0
 */

#include <string>

#include "../../../../beans/gpd/GPDType.h"
#include "../../../../beans/List.h"
#include "../../../../utils/type/PhysicalType.h"
#include "../../DVCS/DVCSObservable.h"
#include "../../../gpd/GPDModule.h"

namespace PARTONS {

/**
 * @class ElasticFFr2E_NEUTRON
 * @brief Elastic F1.
 *
 * Unit: none.
 */
class ElasticFFr2E_NEUTRON: public DVCSObservable {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Integrand.
     */
    static double integrand(double x, void* params);

    /**
     * Constructor.
     * @param className Name of class.
     */
    ElasticFFr2E_NEUTRON(const std::string &className);

    /** 
     * Integration parameters.
     */
    struct Parameters{

        GPDModule* m_pGPDModule;
        double m_xi;
        double m_t;
        double m_muF2;
        double m_muR2;

        GPDType::Type  m_gpdType;
    };

    /**
     * Destructor.
     */
    virtual ~ElasticFFr2E_NEUTRON();

    virtual ElasticFFr2E_NEUTRON* clone() const;

    virtual PhysicalType<double> computeObservable(
            const DVCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    ElasticFFr2E_NEUTRON(const ElasticFFr2E_NEUTRON &other);
};

} /* namespace PARTONS */

#endif /* DVCSCFFXIREH_H */
