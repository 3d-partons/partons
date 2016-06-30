#include "../../../../include/partons/utils/exceptions/CCFModuleNullPointerException.h"

CCFModuleNullPointerException::CCFModuleNullPointerException(
        const std::string &msg) :
        m_msg(msg) {
}

CCFModuleNullPointerException::~CCFModuleNullPointerException() throw () {
}

const char* CCFModuleNullPointerException::what() const throw () {
    return m_msg.c_str();
}
