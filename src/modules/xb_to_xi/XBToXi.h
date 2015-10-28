/*
 * XBToXi.h
 *
 *  Created on: Oct 28, 2015
 *      Author: debian
 */

#ifndef XBTOXI_H_
#define XBTOXI_H_

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

#endif /* XBTOXI_H_ */
