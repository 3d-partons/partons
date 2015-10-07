/*
 * XbToXiConverter.cpp
 *
 *  Created on: Oct 5, 2015
 *      Author: debian
 */

#include "XbToXiConverter.h"

#include "../../BaseObjectRegistry.h"

// Initialise [class]::classId with a unique name.
const unsigned int XbToXiConverter::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new XbToXiConverter("XbToXiConverter"));

XbToXiConverter::XbToXiConverter(const std::string &className) :
        ModuleObject(className) {
}

XbToXiConverter* XbToXiConverter::clone() const {
    return new XbToXiConverter(*this);
}

XbToXiConverter::~XbToXiConverter() {
}

XbToXiConverter::XbToXiConverter(const XbToXiConverter& other) :
        ModuleObject(other) {
}

void XbToXiConverter::isModuleWellConfigured() {
}

void XbToXiConverter::initModule() {
}
