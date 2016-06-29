#include "../../../../include/partons/utils/exceptions/GPDModuleNullPointerException.h"

#include <ElementaryUtils/string_utils/StringUtils.h>

GPDModuleNullPointerException::GPDModuleNullPointerException(
        const std::string &msg) :
        std::exception(), m_msg(ElemUtils::StringUtils::EMPTY) {
}

GPDModuleNullPointerException::~GPDModuleNullPointerException() throw () {
}

const char* GPDModuleNullPointerException::what() const throw () {
    return m_msg.c_str();
}
