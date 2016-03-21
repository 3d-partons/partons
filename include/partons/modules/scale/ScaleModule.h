#ifndef SCALE_MODULE_H
#define SCALE_MODULE_H

/**
 * @file ScaleModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 28 October 2015
 * @version 1.0
 *
 * @class ScaleModule
 *
 * @brief
 */

#include <string>

#include "../../ModuleObject.h"

class Scale;

class ScaleModule: public ModuleObject {
public:
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

protected:
    /**
     * Copy constructor
     */
    ScaleModule(const ScaleModule &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();
};

#endif /* SCALE_MODULE_H */
