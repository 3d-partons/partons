#ifndef QT4_CRYPTOGRAPHIC_HASH_H
#define QT4_CRYPTOGRAPHIC_HASH_H

/**
 * @file Qt4CryptographicHash.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date March 24, 2016
 * @version 1.0
*/

#include <string>

#include "CryptographicHashI.h"

/**
 * @class Qt4CryptographicHash
 *
 * @brief Class for generating hash values based on Qt4 library.
 *
 * This class can be used for generating of hash values. It is based on Qt4 library (`QCryptographicHash` class).
 */
class Qt4CryptographicHash: public CryptographicHashI {

public:

	/**
	 * Default constructor.
	 */
    Qt4CryptographicHash();

    /**
     * Destructor.
     */
    virtual ~Qt4CryptographicHash();

    virtual std::string generateSHA1HashSum(const std::string &string) const;
};

#endif /* QT4_CRYPTOGRAPHIC_HASH_H */
