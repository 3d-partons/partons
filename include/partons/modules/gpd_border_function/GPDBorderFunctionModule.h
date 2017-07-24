/*
 * GPDBorderFunctionModule.h
 *
 *  Created on: Oct 17, 2016
 *      Author: Pawel Sznjader (IPNO)
 */

#ifndef GPD_BORDER_FUNCTION_MODULE_H_
#define GPD_BORDER_FUNCTION_MODULE_H_

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>

#include "../../beans/gpd/GPDBorderFunctionKinematic.h"
#include "../../beans/gpd/GPDType.h"
#include "../gpd/GPDModule.h"

namespace PARTONS {

/**
 * @class GPDBorderFunctionModule
 *
 * @brief Abstract class for modeling of GPD border functions.
 *
 * This class acts as an abstract (mother) class for modules used in the modeling of the GPD border functions, i.e. GPD at (\f$\xi\f$, \f$\xi\f$, \f$t\f$, \f$\mu_{F}^{2}\f$, \f$\mu_{R}^{2}\f$) kinematics.
 * The usage of such kind of modules is illustrated by the following example:
 \code{.cpp}
 // Retrieve GPD service
 GPDService* pGPDService = Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

 // Load GPDBorderFunction module with the BaseModuleFactory
 GPDBorderFunctionModule* pGPDBorderFunctionModel = Partons::getInstance()->getModuleObjectFactory()->newGPDBorderFunctionModule(KM10BorderFunctionModel::classId);

 // Create border function kinematics
 GPDBorderFunctionKinematic borderFunctionKinematic(0.001, -.3, 2., 2.);

 // Compute
 GPDResult gpdResult = pGPDService->computeGPDModel(borderFunctionKinematic, pGPDBorderFunctionModel);

 // Print results
 Partons::getInstance()->getLoggerManager()->info("main", __func__, gpdResult.toString());
 \endcode
 which gives via Logger:
 \code
 06-06-2017 07:38:31 [INFO] (main::main) [GPDResult]
 m_className = GPDResult - m_objectId = 197 indexId = -1
 [GPDKinematic]
 m_className = GPDKinematic - m_objectId = 195 indexId = -1
 m_x = 0.001 m_xi = 0.001 m_t = -0.3 m_MuF2 = 2(Gev2) m_MuR2 = 2(Gev2)
 [PartonDistributionList]
 GPD_H
 [PartonDistribution]
 m_className = PartonDistribution - m_objectId = 236 indexId = -1
 GluonDistribution = 0
 u = 111.217
 u(+) = 216.409
 u(-) = 6.02411
 d = 108.205
 d(+) = 213.397
 d(-) = 3.01206
 s = 0
 s(+) = 0
 s(-) = 0
 \endcode
 */
class GPDBorderFunctionModule: public GPDModule {

public:

    /**
     * Constructor.
     * @param className Name of class.
     */
    GPDBorderFunctionModule(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~GPDBorderFunctionModule();

    virtual GPDBorderFunctionModule* clone() const = 0;

    virtual void run();

    void virtual configure(const ElemUtils::Parameters &parameters);

    /**
     * Evaluate specific GPD border function for a given kinematics and (eventually) the evolution.
     * @param kinematic Reference to the object containing kinematics to be used in the evaluation.
     * @param gpdType Type of GPD.
     * @param evolution Switch to indicate if GPD evolution module should be used in the evaluation.
     * @return Result of the evaluation.
     */
    virtual PartonDistribution compute(
            const GPDBorderFunctionKinematic &kinematic, GPDType gpdType,
            bool evolution = false);

    /**
     * Evaluate specific GPD border function for a given kinematics and (eventually) the evolution.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     * @param gpdType Type of GPD.
     * @param evolution Switch to indicate if GPD evolution module should be used in the evaluation.
     * @return Result of the evaluation.
     */
    virtual PartonDistribution compute(double xi, double t, double MuF2,
            double MuR2, GPDType::Type gpdType, bool evolution = false);

    virtual PartonDistribution computeH();
    virtual PartonDistribution computeE();
    virtual PartonDistribution computeHt();
    virtual PartonDistribution computeEt();

    virtual std::string toString() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    GPDBorderFunctionModule(const GPDBorderFunctionModule &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();
};

} /* namespace PARTONS */

#endif /* GPD_BORDER_FUNCTION_MODULE_H_ */
