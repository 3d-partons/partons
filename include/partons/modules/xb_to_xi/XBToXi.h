#ifndef XB_TO_XI_H
#define XB_TO_XI_H

/**
 * @file XBToXi.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 05 October 2015
 * @version 1.0
 *
 * @class XBToXi
 *
 * @brief
 */

#include <string>

#include "XiConverterModule.h"

class XBToXi: public XiConverterModule {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    XBToXi(const std::string &className);
    virtual ~XBToXi();

    virtual XBToXi* clone() const;

    virtual double compute(double xB, double t, double Q2);

protected:
    /**
     * Copy constructor
     */
    XBToXi(const XBToXi &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();
};

#endif /* XB_TO_XI_H */
