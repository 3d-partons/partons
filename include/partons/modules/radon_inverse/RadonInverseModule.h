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

#include "../../ModuleObject.h"

class RadonInverseModule: public ModuleObject {
public:
    RadonInverseModule();
    virtual ~RadonInverseModule();

    /**
     * Clone
     *
     * @return
     */
    virtual RadonInverseModule* clone() const;



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
