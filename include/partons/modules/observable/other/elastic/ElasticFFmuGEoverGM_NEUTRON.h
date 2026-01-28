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
 * @class ElasticFFmuGEoverGM_NEUTRON
 * @brief Elastic F1.
 *
 * Unit: none.
 */
class ElasticFFmuGEoverGM_NEUTRON: public DVCSObservable {

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
    ElasticFFmuGEoverGM_NEUTRON(const std::string &className);

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
    virtual ~ElasticFFmuGEoverGM_NEUTRON();

    virtual ElasticFFmuGEoverGM_NEUTRON* clone() const;

    virtual PhysicalType<double> computeObservable(
            const DVCSObservableKinematic& kinematic,
            const List<GPDType>& gpdType);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    ElasticFFmuGEoverGM_NEUTRON(const ElasticFFmuGEoverGM_NEUTRON &other);
};

} /* namespace PARTONS */

#endif /* DVCSCFFXIREH_H */
