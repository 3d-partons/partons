#ifndef CRYPTOGRAPHIC_HASH_I_H
#define CRYPTOGRAPHIC_HASH_I_H

/**
 * @file CryptographicHashI.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date March 24, 2016
 * @version 1.0
 */

#include <string>

#include "../../BaseObject.h"

namespace PARTONS {

/**
 * @class CryptographicHashI
 *
 * @brief Abstract class for modules generating hash values.
 *
 * This class acts as an abstract (mother) class for modules used in the generation of hash values for input strings of characters.
 */
class CryptographicHashI: public BaseObject {

public:

	/**
	 * Constructor.
	 * @param className Name of class.
	 */
    CryptographicHashI(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~CryptographicHashI();

    /**
     * Generate SHA-1 hash value for a given string of characters.
     * @param string Input string.
     */
    virtual std::string generateSHA1HashSum(
            const std::string &string) const = 0;
};

} /* namespace PARTONS */

#endif /* CRYPTOGRAPHIC_HASH_I_H */
