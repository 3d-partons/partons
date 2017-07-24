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

namespace PARTONS {

/**
 * @class CCFModuleNullPointerException
 *
 * @brief Exception to indicate missing CFF module.
 *
 * This class acts as an exception used whenever needed to indicate a missing CFF module in the computation configuration.
 */
class CCFModuleNullPointerException: public std::exception {
public:

	/**
	 * Constructor.
	 * @param msg Message to be assigned to this exception.
	 */
    CCFModuleNullPointerException(const std::string &msg);

    /**
     * Destructor.
     */
    virtual ~CCFModuleNullPointerException() throw ();

    virtual const char* what() const throw ();

private:

    /**
     * Message assigned to this exception.
     */
    std::string m_msg;
};

} /* namespace PARTONS */

#endif /* CCF_MODULE_NULL_POINTER_EXCEPTION_H */
