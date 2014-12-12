#ifndef ROOT_INTEGRATION_MODE_H
#define ROOT_INTEGRATION_MODE_H

/**
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 11 december 2014
 * @version 1.0
 */

#include <string>

#include "../MathIntegratorModule.h"

class RootIntegrationMode: public MathIntegratorModule {
public:
    static const std::string moduleID; ///< Unique ID to automatically register the module in the factory.

    RootIntegrationMode();
    virtual ~RootIntegrationMode();

    virtual double compute(unsigned int functionName,
            ModuleObject* pModuleObject, double xMin, double xMax);

    virtual RootIntegrationMode* clone() const;

protected:
    /***
     * Copy constructor
     * @param other
     */
    RootIntegrationMode(const RootIntegrationMode &other);

    virtual void isModuleWellConfigured();
    virtual void initModule();
};

#endif /* ROOT_INTEGRATION_MODE_H */
