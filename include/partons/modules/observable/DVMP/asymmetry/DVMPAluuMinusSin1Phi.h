#ifndef DVMPALUUMINUSSIN1PHI
#define DVMPALUUMINUSSIN1PHI

/**
 * @file DVMPAluuMinusSin1Phi.h
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
#include "DVMPAluuMinus.h"

namespace PARTONS {
class DVMPObservableKinematic;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class DVMPAluuMinusSin1Phi
 *
 * @brief 1th Fourier moment of longitudinally polarized target asymmetry for negative beam charge.
 */
class DVMPAluuMinusSin1Phi: public DVMPAluuMinus, public MathIntegratorModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVMPAluuMinusSin1Phi(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVMPAluuMinusSin1Phi();

    virtual DVMPAluuMinusSin1Phi* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVMPAluuMinusSin1Phi(const DVMPAluuMinusSin1Phi &other);

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

#endif /* DVMPALUUMINUSSIN1PHI */
