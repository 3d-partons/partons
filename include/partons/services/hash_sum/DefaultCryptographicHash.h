#ifndef DEFAULT_CRYPTOGRAPHIC_HASH_H
#define DEFAULT_CRYPTOGRAPHIC_HASH_H

/**
 * @file DefaultCryptographicHash.h
 * @author Pawel Sznajder (NCBJ)
 * @date March 24, 2025
 * @version 1.0
*/

#include <string>

#include "CryptographicHashI.h"

namespace PARTONS {

/**
 * @class DefaultCryptographicHash
 *
 * @brief Class for generating hash values.
 *
 * This class can be used for generating of hash values.
 */
class DefaultCryptographicHash: public CryptographicHashI {

public:

	/**
	 * Default constructor.
	 */
    DefaultCryptographicHash();

    /**
     * Destructor.
     */
    virtual ~DefaultCryptographicHash();

    virtual std::string generateSHA1HashSum(const std::string &string) const;
};

} /* namespace PARTONS */

#endif /* DEFAULT_CRYPTOGRAPHIC_HASH_H */
