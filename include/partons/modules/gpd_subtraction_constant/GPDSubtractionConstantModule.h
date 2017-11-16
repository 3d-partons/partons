/*
 * GPDSubtractionConstantModule.h
 *
 *  Created on: Oct 18, 2016
 *      Author: debian
 */

#ifndef GPD_SUBTRACTION_CONSTANT_MODULE_H_
#define GPD_SUBTRACTION_CONSTANT_MODULE_H_

#include <ElementaryUtils/parameters/Parameters.h>
#include <map>
#include <string>

#include "../../beans/automation/BaseObjectData.h"
#include "../../beans/gpd/GPDBorderFunctionKinematic.h"
#include "../../ModuleObject.h"

namespace PARTONS {

/**
 * @class GPDSubtractionConstantModule
 *
 * @brief Abstract class for modeling of GPD subtraction constant.
 *
 * This class acts as an abstract (mother) class for modules used in the modeling of the GPD subtraction constants.
 * The usage of such kind of modules is illustrated by the following example:
 \code{.cpp}
 // Retrieve GPD service
 GPDService* pGPDService = Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

 // Load GPDBorderFunction module with the BaseModuleFactory
 GPDSubtractionConstantModule* pGPDSubtractionConstantModule = Partons::getInstance()->getModuleObjectFactory()->newGPDSubtractionConstantModule(KM10SubtractionConstantModel::classId);

 // Create border function kinematics
 GPDBorderFunctionKinematic borderFunctionKinematic(0.001, -0.3, 2., 2.);

 // Compute
 double subConst = pGPDSubtractionConstantModule->compute(borderFunctionKinematic, GPDType::H);

 // Print results
 Partons::getInstance()->getLoggerManager()->info("main", __func__, ElemUtils::Formatter() << "subtraction constant for kinematics" << borderFunctionKinematic.toString() << " is: " << subConst);
 \endcode
 which gives via Logger:
 \code
 06-06-2017 07:59:46 [INFO] (main::main) subtraction constant for kinematics[GPDKinematic]
 m_className = GPDKinematic - m_objectId = 195 indexId = -1
 m_x = 0.001 m_xi = 0.001 m_t = -0.3 m_MuF2 = 2(Gev2) m_MuR2 = 2(Gev2) is: 4.67128027681661
 \endcode
 */
class GPDSubtractionConstantModule: public ModuleObject {

public:

    /**
     * Type of module name used by the automatization.
     */
    static const std::string GPD_SUBTRACTION_CONSTANT_MODULE_CLASS_NAME;

    /**
     * Constructor.
     * @param className Name of class.
     */
    GPDSubtractionConstantModule(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~GPDSubtractionConstantModule();

    virtual GPDSubtractionConstantModule* clone() const;
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual std::string toString() const;
    virtual void resolveObjectDependencies();
    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

    /**
     * Evaluate specific GPD subtraction constant for a given kinematics.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    double compute(double t, double MuF2, double MuR2);

    /**
     * Evaluate specific GPD subtraction constant for a given GPDBorderFunctionKinematic object.
     * @param gpdBorderFunctionKinematic GPDBorderFunctionKinematic object containing kinematics to be evaluated.
     */
    double compute(
            const GPDBorderFunctionKinematic& gpdBorderFunctionKinematic);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    GPDSubtractionConstantModule(const GPDSubtractionConstantModule &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    /**
     * Evaluate specific GPD subtraction constant (user defined part).
     */
    virtual double computeSubtractionConstant();

    /**
     * Evaluate specific GPD subtraction constant for a given kinematics.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    void preCompute(double t, double MuF2, double MuR2);

    /**
     * Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     */
    double m_t;

    /**
     * Factorization scale squared (in \f$GeV^{2}\f$).
     */
    double m_MuF2;

    /**
     * Renormalization scale squared (in \f$GeV^{2}\f$).
     */
    double m_MuR2;
};

} /* namespace PARTONS */

#endif /* GPD_SUBTRACTION_CONSTANT_MODULE_H_ */
