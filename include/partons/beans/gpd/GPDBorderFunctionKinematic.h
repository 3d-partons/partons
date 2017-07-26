/*
 * GPDBorderFunctionKinematic.h
 *
 *  Created on: Oct 17, 2016
 *      Author: Pawel Sznajder (IPNO)
 */

#ifndef GPDBORDERFUNCTIONKINEMATIC_H_
#define GPDBORDERFUNCTIONKINEMATIC_H_

#include <ElementaryUtils/parameters/GenericType.h>

#include "GPDKinematic.h"

namespace PARTONS {

/**
 * @class GPDBorderFunctionKinematic
 *
 * @brief Class representing single GPD border function kinematics.
 *
 * This class represents a single GPD border function kinematics (xi, xi, t, \f$\mu_{F}^{2}\f$, \f$\mu_{F}^{2}\f$). It is a derivative of GPDKinematic, where x = xi.
 * Analyze the following code for the example of usage:
 \code{.cpp}
 //evaluate exemplary GPD result

 //retrieve GPD service
 GPDService* pGPDService = Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

 //load GPD module with the BaseModuleFactory
 GPDBorderFunctionModule* gpdBorderFunctionModule = Partons::getInstance()->getModuleObjectFactory()->newGPDBorderFunctionModule(KM10BorderFunctionModel::classId);

 //define GPD kinematics to be used in computation
 //here we are using the constructor that allows us to assign values of x = xi, t, muF2 and muR2 immediately
 GPDBorderFunctionKinematic borderFunctionKinematic(0.05, 0., 2., 2.);

 //define list of GPD types to be computed
 List<GPDType> gpdTypeList;
 gpdTypeList.add(GPDType::H);

 //evaluate
 GPDResult borderFunctionResult = pGPDService->computeGPDModel(borderFunctionKinematic, gpdBorderFunctionModule, gpdTypeList);

 //print result
 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Kinematics:\n" << borderFunctionKinematic.toString());
 Partons::getInstance()->getLoggerManager()->info("example", __func__, ElemUtils::Formatter() << "Result:\n" << borderFunctionResult.toString());
 \endcode
 */
class GPDBorderFunctionKinematic: public GPDKinematic {

public:

    /**
     * Default constructor.
     */
    GPDBorderFunctionKinematic();

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    GPDBorderFunctionKinematic(const GPDBorderFunctionKinematic &other);

    /**
     * Assignment constructor.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    GPDBorderFunctionKinematic(double xi, double t, double MuF2, double MuR2);

    /**
     * Assignment constructor.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    GPDBorderFunctionKinematic(const ElemUtils::GenericType &xi,
            const ElemUtils::GenericType &t, const ElemUtils::GenericType &MuF2,
            const ElemUtils::GenericType &MuR2);

    /**
     * Assignment constructor.
     * @param parameters Parameters object storing values to be set marked by GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_XI, ObservableKinematic::PARAMETER_NAME_T, GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUF2, GPDKinematic::GPD_KINEMATIC_PARAMETER_NAME_MUR2.
     */
    GPDBorderFunctionKinematic(const ElemUtils::Parameters &parameters);

    /**
     * Destructor.
     */
    virtual ~GPDBorderFunctionKinematic();

    virtual std::string toString() const;

protected:

    virtual void updateHashSum() const;
};

} /* namespace PARTONS */

#endif /* GPDBORDERFUNCTIONKINEMATIC_H_ */
