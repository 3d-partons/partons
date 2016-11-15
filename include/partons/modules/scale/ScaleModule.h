#ifndef SCALE_MODULE_H
#define SCALE_MODULE_H

/**
 * @file ScaleModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date October 28, 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <map>
#include <string>

#include "../../beans/automation/BaseObjectData.h"
#include "../../ModuleObject.h"

class Scale;

/**
 * @class ScaleModule
 *
 * @brief
 */
class ScaleModule: public ModuleObject {
public:
    static const std::string SCALE_MODULE_CLASS_NAME;

    ScaleModule(const std::string &className);
    virtual ~ScaleModule();
    /**
     * Clone
     *
     * @return
     */
    virtual ScaleModule* clone() const = 0;

    virtual Scale compute(double Q2) = 0;

    virtual void configure(const ElemUtils::Parameters &parameters);

    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

protected:
    /**
     * Copy constructor
     */
    ScaleModule(const ScaleModule &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();
};

#endif /* SCALE_MODULE_H */
