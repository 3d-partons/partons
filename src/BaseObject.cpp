#include "BaseObject.h"

BaseObject::BaseObject(std::string className) :
        m_className(className) {

}

BaseObject::~BaseObject() {

}

const std::string BaseObject::getClassName() {
    return m_className;
}
