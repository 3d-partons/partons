#ifndef DVMPAULUMINUSSIN1PHI
#define DVMPAULUMINUSSIN1PHI

/**
 * @file DVMPAuluMinusSin1Phi.h
 * @author Pawel Sznajder (NCBJ, Warsaw)
 * @date November 16, 2017
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>

#include "../../../../beans/gpd/GPDType.h"
#include "../../../../beans/List.h"
#include "../../../../utils/type/PhysicalType.h"
#include "../../../MathIntegratorModule.h"
#include "DVMPAuluMinus.h"

namespace PARTONS {
class DVMPObservableKinematic;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class DVMPAuluMinusSin1Phi
 *
 * @brief 1th Fourier moment of longitudinally polarized target asymmetry for negative beam charge.
 */
class DVMPAuluMinusSin1Phi: public DVMPAuluMinus, public MathIntegratorModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVMPAuluMinusSin1Phi(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVMPAuluMinusSin1Phi();

    virtual DVMPAuluMinusSin1Phi* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVMPAuluMinusSin1Phi(const DVMPAuluMinusSin1Phi &other);

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
};

} /* namespace PARTONS */

#endif /* DVMPAULUMINUSSIN1PHI */
