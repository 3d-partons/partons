#ifndef GPD_MODULE_NULL_POINTER_EXCEPTION_H
#define GPD_MODULE_NULL_POINTER_EXCEPTION_H

/**
 * @file GPDModuleNullPointerException.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date June 29, 2016
 * @version 1.0
 */

#include <exception>
#include <string>

/**
 * @class GPDModuleNullPointerException
 *
 * @brief
 */
class GPDModuleNullPointerException: public std::exception {
public:
    GPDModuleNullPointerException(const std::string &msg);
    virtual ~GPDModuleNullPointerException() throw ();

    virtual const char* what() const throw ();

private:
    std::string m_msg;
};

#endif /* GPD_MODULE_NULL_POINTER_EXCEPTION_H */
