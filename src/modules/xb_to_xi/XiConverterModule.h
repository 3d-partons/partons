#ifndef XI_CONVERTER_MODULE
#define XI_CONVERTER_MODULE

/**
 * @file XiConverterModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 05 October 2015
 * @version 1.0
 *
 * @class XiConverterModule
 *
 * @brief
 */

#include <string>

#include "../../ModuleObject.h"

class XiConverterModule: public ModuleObject {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    XiConverterModule(const std::string &className);
    virtual ~XiConverterModule();

    virtual XiConverterModule* clone() const = 0;

    virtual double compute(double xB, double t, double Q2) = 0;
protected:
    /**
     * Copy constructor
     *
     * Use by the factory
     *
     * @param other
     */
    XiConverterModule(const XiConverterModule& other);

    virtual void isModuleWellConfigured();
    virtual void initModule();
};

#endif /* XI_CONVERTER_MODULE */
