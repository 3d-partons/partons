#ifndef OBSERVABLE_H
#define OBSERVABLE_H

/**
 * @file Observable.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 25, 2014
 * @version 1.0
 */

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <map>
#include <string>

#include "../../beans/automation/BaseObjectData.h"
#include "../../beans/channel/ChannelType.h"
#include "../../beans/gpd/GPDType.h"
#include "../../beans/List.h"
#include "../../ModuleObject.h"
#include "../../utils/type/PhysicalType.h"

namespace PARTONS {

/**
 * @class Observable
 *
 * @brief Abstract class that provides a skeleton to implement an Observable module.
 */
template<typename KinematicType, typename ResultType>
class Observable: public ModuleObject {

public:

    /**
     * Destructor
     */
    virtual ~Observable() {
    }

    virtual Observable* clone() const = 0;

    virtual std::string toString() const {
        return ModuleObject::toString();
    }

    virtual void resolveObjectDependencies() {
        ModuleObject::resolveObjectDependencies();
    }

    virtual void run() {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "This must be implemented in daughter class");
    }

    virtual void configure(const ElemUtils::Parameters &parameters) {
        ModuleObject::configure(parameters);
    }

    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData) {
        ModuleObject::prepareSubModules(subModulesData);
    }

    /**
     * Computes the observable at given kinematics.
     * @param kinematic Kinematics.
     * @param gpdType Type of GPDs to compute.
     * @return Result.
     */
    virtual ResultType compute(const KinematicType& kinematic,
            const List<GPDType> & gpdType = List<GPDType>()) = 0;

    /**
     * Must be implemented in child class.
     * @return List of GPD/CCF types the child class can compute.
     */
    virtual List<GPDType> getListOfAvailableGPDTypeForComputation() const = 0;

protected:

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     * @param channelType Channel type.
     */
    Observable(const std::string &className, ChannelType::Type channelType) :
            ModuleObject(className, channelType) {
    }

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    Observable(const Observable& other) :
            ModuleObject(other) {
    }

    /**
     * Set internal kinematics
     * @param kinematic Kinematics to be set
     */
    virtual void setKinematics(const KinematicType& kinematic) = 0;

    virtual void initModule() {
    }

    virtual void isModuleWellConfigured() {
    }

    /**
     * Evaluate observable. To be implemented in a child class.
     */
    virtual PhysicalType<double> computeObservable(
            const KinematicType& kinematic, const List<GPDType>& gpdType) = 0;
};

} /* namespace PARTONS */

#endif /* OBSERVABLE_H */
