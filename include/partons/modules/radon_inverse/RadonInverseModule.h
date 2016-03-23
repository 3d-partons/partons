#ifndef RADON_INVERSE_MODULE_H
#define RADON_INVERSE_MODULE_H

/**
 * @file RadonInverseModule.h
 * @author Cedric MEZRAG
 * @date 18 March 2016
 * @version 1.0
 *
 * @class RadonInverseModule
 *
 * @brief
 */

#include <string>

#include "../../ModuleObject.h"

class RadonInverseModule: public ModuleObject {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    RadonInverseModule(const std::string &className);
    virtual ~RadonInverseModule();

    /**
     * Clone
     *
     * @return
     */
    virtual RadonInverseModule* clone() const;

    virtual void configure(const ElemUtils::Parameters &parameters);
protected:
    /**
     * Copy constructor
     */
    RadonInverseModule(const RadonInverseModule &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

private:
};

#endif /* RADON_INVERSE_MODULE_H */
