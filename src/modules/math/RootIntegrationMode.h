#ifndef ROOT_INTEGRATION_MODE_H
#define ROOT_INTEGRATION_MODE_H

/**
 * @file RootIntegrationMode.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 11 December 2014
 * @version 1.0
 *
 * @class RootIntegrationMode
 *
 * @brief
 */

#include <string>

#include "../MathIntegratorModule.h"

class RootIntegrationMode: public MathIntegratorModule {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    RootIntegrationMode(const std::string &className);
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
