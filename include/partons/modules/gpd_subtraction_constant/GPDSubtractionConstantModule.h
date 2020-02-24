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
#include "../../beans/gpd/GPDType.h"
#include "../../ModuleObject.h"
#include "../../utils/type/PhysicalType.h"

namespace PARTONS {
class GPDSubtractionConstantKinematic;
class GPDSubtractionConstantResult;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class GPDSubtractionConstantModule
 *
 * @brief Abstract class for modeling of GPD subtraction constant.
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
    virtual std::string toString() const;
    virtual void resolveObjectDependencies();
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

    /**
     * Evaluate specific GPD subtraction constant for a given kinematics.
     * @param kinematic GPD kinematics
     * @param gpdType GPD type
     */
    virtual GPDSubtractionConstantResult compute(
            const GPDSubtractionConstantKinematic& kinematic,
            GPDType::Type gpdType);

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
    virtual PhysicalType<double> computeSubtractionConstant();

    /**
     * Set internal kinematics.
     * @param kinematic Kinematics to be set.
     */
    virtual void setKinematics(
            const GPDSubtractionConstantKinematic& kinematic);

    /**
     * Set current GPD type to be computed.
     */
    void setCurrentGPDType(GPDType::Type gpdType);

    double m_t; ///< Mandelstam variable, momentum transfer on the hadron target.
    double m_MuF2; ///< Factorization scale squared.
    double m_MuR2; ///< Renormalization scale squared.
    GPDType::Type m_currentGPDComputeType; ///< Current GPD type.
};

} /* namespace PARTONS */

#endif /* GPD_SUBTRACTION_CONSTANT_MODULE_H_ */
