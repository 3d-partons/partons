/*
 * XbToXiConverter.h
 *
 *  Created on: Oct 5, 2015
 *      Author: debian
 */

#ifndef XBTOXICONVERTER_H_
#define XBTOXICONVERTER_H_

#include <string>

#include "../../ModuleObject.h"

class XbToXiConverter: public ModuleObject {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    XbToXiConverter(const std::string &className);
    virtual ~XbToXiConverter();

    virtual XbToXiConverter* clone() const;
protected:
    /**
     * Copy constructor
     *
     * Use by the factory
     *
     * @param other
     */
    XbToXiConverter(const XbToXiConverter& other);

    virtual void isModuleWellConfigured();
    virtual void initModule();
};

#endif /* XBTOXICONVERTER_H_ */
