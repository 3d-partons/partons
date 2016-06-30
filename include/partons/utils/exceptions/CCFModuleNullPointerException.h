#ifndef CCF_MODULE_NULL_POINTER_EXCEPTION_H
#define CCF_MODULE_NULL_POINTER_EXCEPTION_H

/**
 * @file CCFModuleNullPointerException.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date June 30, 2016
 * @version 1.0
 */

#include <exception>
#include <string>

/**
 * @class CCFModuleNullPointerException
 *
 * @brief
 */
class CCFModuleNullPointerException: public std::exception {
public:
    CCFModuleNullPointerException(const std::string &msg);
    virtual ~CCFModuleNullPointerException() throw ();

    virtual const char* what() const throw ();

private:
    std::string m_msg;
};

#endif /* CCF_MODULE_NULL_POINTER_EXCEPTION_H */
