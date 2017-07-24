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

namespace PARTONS {

/**
 * @class GPDModuleNullPointerException
 *
 * @brief Exception to indicate missing GPD module.
 *
 * This class acts as an exception used whenever needed to indicate a missing GPD module in the computation configuration.
 */
class GPDModuleNullPointerException: public std::exception {

public:

	/**
	 * Constructor.
	 * @param msg Message to be assigned to this exception.
	 */
    GPDModuleNullPointerException(const std::string &msg);

    /**
     * Destructor.
     */
    virtual ~GPDModuleNullPointerException() throw ();

    virtual const char* what() const throw ();

private:

    /**
     * Message assigned to this exception.
     */
    std::string m_msg;
};

} /* namespace PARTONS */

#endif /* GPD_MODULE_NULL_POINTER_EXCEPTION_H */
