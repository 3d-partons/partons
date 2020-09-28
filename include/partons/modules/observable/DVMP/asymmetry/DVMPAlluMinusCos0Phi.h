#ifndef DVMPALLUMINUSCOS0PHI
#define DVMPALLUMINUSCOS0PHI

/**
 * @file DVMPAlluMinusCos0Phi.h
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
#include "DVMPAlluMinus.h"

namespace PARTONS {
class DVMPObservableKinematic;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class DVMPAlluMinusCos0Phi
 *
 * @brief 0th Fourier moment of longitudinally polarized beam - target asymmetry for negative beam charge.
 */
class DVMPAlluMinusCos0Phi: public DVMPAlluMinus, public MathIntegratorModule {

public:

    /**
     * Unique ID to automatically register the class in the registry.
     */
    static const unsigned int classId;

    /**
     * Constructor.
     * @param className Name of class.
     */
    DVMPAlluMinusCos0Phi(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~DVMPAlluMinusCos0Phi();

    virtual DVMPAlluMinusCos0Phi* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    DVMPAlluMinusCos0Phi(const DVMPAlluMinusCos0Phi &other);

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

#endif /* DVMPALLUMINUSCOS0PHI */
